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
	//ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	AAIController* AGVAICon = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_Find : 블랙보드를 가져오질 못해서 작업 종료할게!"));
		return EBTNodeResult::Failed;
	}

	// AGV의 현재 작업 단계(AGV.Phase)를 블랙보드에서 가져온다.
	FGameplayTag CurrentAGVPhaseTag = FGameplayTag::RequestGameplayTag(BlackboardComp->GetValueAsName(CurrentAGVPhaseKey));
	if (!CurrentAGVPhaseTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("BTTask_Find : 태그를 못찾았어!"));
		return EBTNodeResult::Failed;
	}

	// AGV가 가야할 셀 타입 검출
	FGameplayTag CellTypeTag = GetCellTypeTagsForPhase(CurrentAGVPhaseTag);

	UE_LOG(LogTemp, Log, TEXT("BTTask_Find : 검출 셀 태그 이름 %s"), *CellTypeTag.GetTagName().ToString());
	ADFCellBase* MoveToCell = nullptr;

	// 탐색
	for (TActorIterator<ADFCellBase> It(GetWorld()); It; ++It)
	{
		ADFCellBase* CurrentCell = *It;
		// 검출한 셀 타입과 탐색한 셀 타입이 일치하는지 체크(Cell.Type)
		if (CurrentCell->CellTypeTag.MatchesTag(CellTypeTag))
		{
			UE_LOG(LogTemp, Log, TEXT("BTTask_Find : 여기 들어오니!"));
			UDFAbilitySystemComponent* CellASC = CurrentCell->GetDFAbilitySystemComponent();
			UE_LOG(LogTemp, Log, TEXT("BTTask_Find : 이셀 무슨 타입이야? %s"), *CellASC->GetCurrentCellStateTag().ToString());
			if (CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")) &&
				!CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending")) &&
				!CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
			{
				// 하나만 찾으면 된다.
				UE_LOG(LogTemp, Log, TEXT("BTTask_Find : 여기 들어오니?!"));
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
			CellASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
			CellASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));
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
