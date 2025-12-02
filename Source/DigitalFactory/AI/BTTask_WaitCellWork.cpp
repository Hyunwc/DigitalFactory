// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WaitCellWork.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Cell/DFCellBase.h"
#include "DFAI.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Robot/DFAGV.h"
#include "AIController.h"
#include "DFGameplayTags.h"

UBTTask_WaitCellWork::UBTTask_WaitCellWork()
{
	NodeName = TEXT("Wait For Cell Work");
	bNotifyTaskFinished = true; // Task가 종료될 때 OnTaskFinished가 호출되도록 설정
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
	TargetCellKeyName = BBKEY_TARGETCELL;
}

EBTNodeResult::Type UBTTask_WaitCellWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	OwnerCompRef = &OwnerComp;

	CurrentWorkingCell = Cast<ADFCellBase>(BlackboardComp->GetValueAsObject(TargetCellKeyName));

	if (!CurrentWorkingCell)
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_WaitCell : No Target!"));
		return EBTNodeResult::Failed;
	}

	ADFAGV* Owner = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());

	// 셀 작업자 변경 및 예약 해제
	CurrentWorkingCell->SetCellWorkingAGV(Owner);
	CurrentWorkingCell->SetCellReservedAGV(nullptr);
	// Owner(AGV)가 작업중인 셀 변경
	Owner->SetCurrentCell(CurrentWorkingCell);

	// 셀의 작업 완료 델리게이트 바인딩
	CurrentWorkingCell->OnCellWorkComplete.AddDynamic(
		this, &UBTTask_WaitCellWork::OnCellWorkCompleted);
	// 작업 시작 상태 변경
	CurrentWorkingCell->SetCellStateTag(DFGameplayTags::Cell_State_Working);
	Owner->SetAGVStateTag(DFGameplayTags::AGV_State_Work);
	// 셀 작업 시작 호출
	CurrentWorkingCell->StartWork(Owner);

	return EBTNodeResult::InProgress;
}

void UBTTask_WaitCellWork::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	ADFAGV* Owner = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	// Task가 종료될 때 델리게이트 바인딩 해제
	if (CurrentWorkingCell)
	{
		UE_LOG(LogTemp, Log, TEXT("%s : Task 종료! (%s)"), *CurrentWorkingCell->GetName(), *GetNameSafe(OwnerCompRef->GetOwner()));

		CurrentWorkingCell->OnCellWorkComplete.RemoveDynamic(this, &UBTTask_WaitCellWork::OnCellWorkCompleted);

		// 셀에서 참조하고 있는 AGV 해제
		//CurrentWorkingCell->WorkingAGV = nullptr;

		// 참조 해제
		CurrentWorkingCell = nullptr;
		//Owner->CurrentCell = nullptr;
		Owner->SetCurrentCell(nullptr);
		Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	}
}

void UBTTask_WaitCellWork::OnCellWorkCompleted(ADFCellBase* CompletedCell)
{
	// 현재 이 Task가 기다리던 셀의 작업이 완료되었는지 확인
	if (CurrentWorkingCell && CompletedCell == CurrentWorkingCell)
	{
		CompletedCell->SetCellStateTag(DFGameplayTags::Cell_State_Free);
		CompletedCell->SetCellWorkingAGV(nullptr);

		FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCellWorkCompleted Failed. (%s != %s)"), *GetNameSafe(CompletedCell), *GetNameSafe(CurrentWorkingCell));
	}
}


