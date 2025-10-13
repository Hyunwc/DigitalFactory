// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ActivateCellWork.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Cell/DFCellBase.h"
#include "DFAI.h"
#include "Robot/DFAGV.h"
#include "AIController.h"
#include "GAS/DFAbilitySystemComponent.h"

UBTTask_ActivateCellWork::UBTTask_ActivateCellWork()
{
	NodeName = TEXT("Activate Cell Work");
	TargetCellKeyName = BBKEY_TARGETCELL; 
}

EBTNodeResult::Type UBTTask_ActivateCellWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드에서 현재 작업 중인 Cell 액터 레퍼런스 가져온다.
	ADFCellBase* TargetCell = Cast<ADFCellBase>(BlackboardComp->GetValueAsObject(TargetCellKeyName));

	if (!TargetCell)
	{
		//UE_LOG(LogTemp, Error, TEXT("BTTask_Activate : No Target Cell Found %s"), *TargetCellKeyName.ToString());
		return EBTNodeResult::Failed;
	}

	// 셀의 작업 시작 호출
	TargetCell->StartWork(Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn()));
	// Working으로 변경
	if (TargetCell->GetDFAbilitySystemComponent()->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
	{
		UE_LOG(LogTemp, Log, TEXT("BTTask_Activate : 셀 Working으로 변경!"));
	}

	UE_LOG(LogTemp, Log, TEXT("BTTask_Activate : 일 시작하세요! cell %s!"), *TargetCell->GetName());

	return EBTNodeResult::Succeeded;
}
