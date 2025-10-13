// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WaitCellWork.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Cell/DFCellBase.h"
#include "DFAI.h"
#include "GAS/DFAbilitySystemComponent.h"

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

	// 셀의 작업 완료 델리게이트 함수 바인딩
	CurrentWorkingCell->OnCellWorkComplete.AddDynamic(this, &UBTTask_WaitCellWork::OnCellWorkCompleted);

	return EBTNodeResult::InProgress;
}

void UBTTask_WaitCellWork::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// Task가 종료될 때 델리게이트 바인딩 해제
	if (CurrentWorkingCell)
	{
		CurrentWorkingCell->OnCellWorkComplete.RemoveDynamic(this, &UBTTask_WaitCellWork::OnCellWorkCompleted);

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


