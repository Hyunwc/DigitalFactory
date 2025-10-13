// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindNextCell.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/DFAI.h"
#include "Robot/DFAGV.h"
#include "AIController.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Cell/DFCellBase.h"
#include "EngineUtils.h"

UBTTask_FindNextCell::UBTTask_FindNextCell()
{
	NodeName = TEXT("FindNextCell");

	CurrentAGVPhaseKey = BBKEY_CURRENTPHASE;
	NextTargetLocationKey = BBKEY_NEXTPOS;
	CurrentTargetCellKey = BBKEY_TARGETCELL;
}

EBTNodeResult::Type UBTTask_FindNextCell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 만약 Idle이면
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	if (AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("AGV.Phase.Idle"))))
	{
		// Supply로 변경
		AGV->AGVPhaseTag = FGameplayTag::RequestGameplayTag(TEXT("AGV.Phase.Supply"));
	}
	// AGV가 가야할 셀 타입 검출
	FGameplayTag CellTypeTag = GetCellTypeTagsForPhase(AGV->AGVPhaseTag);

	ADFCellBase* MoveToCell = nullptr;

	// 탐색
	for (TActorIterator<ADFCellBase> It(GetWorld()); It; ++It)
	{
		ADFCellBase* CurrentCell = *It;
		// 검출한 셀 타입과 탐색한 셀 타입이 일치하는지 체크(Cell.Type)
		if (CurrentCell->CellTypeTag.MatchesTag(CellTypeTag))
		{
			UDFAbilitySystemComponent* CellASC = CurrentCell->GetDFAbilitySystemComponent();
			if (CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")) &&
				!CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending")) &&
				!CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
			{
				// 하나만 찾으면 된다.
				MoveToCell = CurrentCell;
				break;
			}
		}
	}

	// 예약 걸어두기
	if (MoveToCell)
	{
		UE_LOG(LogTemp, Log, TEXT("BTTask_Find : 예약 걸어둘게요!"));
		UDFAbilitySystemComponent* CellASC = MoveToCell->GetDFAbilitySystemComponent();
		if (CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")))
		{
			//CellASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
			CellASC->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));
		}

		// 블랙보드 Next좌표와 참조 셀 갱신
		BlackboardComp->SetValueAsVector(NextTargetLocationKey, MoveToCell->AGVTargetPoint->GetComponentLocation());
		BlackboardComp->SetValueAsObject(CurrentTargetCellKey, MoveToCell);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

FGameplayTag UBTTask_FindNextCell::GetCellTypeTagsForPhase(const FGameplayTag& AGVPhaseTag) const
{
	if (AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Supply")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.Type.Supply");
	}
	else if (AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Trim")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.Type.Trim");
	}
	else if (AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Flexible")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.Type.Flexible");
	}
	else if (AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Inspection")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.Type.Inspection");
	}

	return FGameplayTag();

}
void UBTTask_FindNextCell::SetCellFree(ADFCellBase* CellToFree) const
{
	if (CellToFree && CellToFree->GetDFAbilitySystemComponent())
	{
		CellToFree->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	}
}
