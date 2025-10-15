// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WaitCellWork.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Cell/DFCellBase.h"
#include "DFAI.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Robot/DFAGV.h"
#include "AIController.h"

UBTTask_WaitCellWork::UBTTask_WaitCellWork()
{
	NodeName = TEXT("Wait For Cell Work");
	bNotifyTaskFinished = true; // Task가 종료될 때 OnTaskFinished가 호출되도록 설정
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

	// 현재 작업 중인 Cell에게 어떤 AGV가 작업중인지 보낸다(어빌리티에서도 사용 및 다른곳에서도 사용할 수 있게)
	CurrentWorkingCell->WorkingAGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());

	// 셀의 작업 완료 델리게이트 함수 바인딩
	CurrentWorkingCell->OnCellWorkComplete.AddDynamic(this, &UBTTask_WaitCellWork::OnCellWorkCompleted);

	// Working으로 변경
	if (CurrentWorkingCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
	{
		UE_LOG(LogTemp, Log, TEXT("BTTask_WaitCellWork : 셀 Working으로 변경!"));
	}
	// 셀 작업 시작 호출
	CurrentWorkingCell->StartWork(Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn()));

	if (CurrentWorkingCell)
	{
		UE_LOG(LogTemp, Log, TEXT("BTTask_WaitCell : 내가 참조하고 있는 셀 이름은! %s"), *CurrentWorkingCell->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("BTTask_WaitCell : 이제 참조중이지 않아"));
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_WaitCellWork::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// Task가 종료될 때 델리게이트 바인딩 해제
	if (CurrentWorkingCell)
	{
		CurrentWorkingCell->OnCellWorkComplete.RemoveDynamic(this, &UBTTask_WaitCellWork::OnCellWorkCompleted);

		// 셀에서 참조하고 있는 AGV 해제
		//CurrentWorkingCell->WorkingAGV = nullptr;

		// 참조 해제
		CurrentWorkingCell = nullptr;

		Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	}
}

void UBTTask_WaitCellWork::OnCellWorkCompleted(ADFCellBase* CompletedCell)
{
	// 현재 이 Task가 기다리던 셀의 작업이 완료되었는지 확인
	if (CurrentWorkingCell && CompletedCell == CurrentWorkingCell)
	{
		UE_LOG(LogTemp, Log, TEXT("%s : 일이 끝났구만!"), *CurrentWorkingCell->GetName());
		if (CompletedCell->GetDFAbilitySystemComponent()->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")))
		{
			UE_LOG(LogTemp, Log, TEXT("UBTTask_WaitCellWork : 셀 이제 Free로 바꿀게!"), *CurrentWorkingCell->GetName());
		}

		FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
	}
}


