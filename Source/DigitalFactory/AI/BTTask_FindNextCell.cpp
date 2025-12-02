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
#include "Kismet/GameplayStatics.h"

UBTTask_FindNextCell::UBTTask_FindNextCell()
{
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
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

	// 만약 목적지가 없다면
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (AGV->AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_None))
	{
		// Supply로 변경
		AGV->SetAGVPhaseTag(DFGameplayTags::AGV_Phase_Supply);
	}
	// AGV가 가야할 셀 타입 검출
	FGameplayTag CellTypeTag = GetCellTypeTagsForPhase(AGV->AGVPhaseTag);

	const int32 TargetCellNum = AGV->FindCellNum;

	ADFCellBase* MoveToCell = nullptr;

	// 탐색
	for (TActorIterator<ADFCellBase> It(GetWorld()); It; ++It)
	{
		ADFCellBase* CurrentCell = *It;
		// 검출한 셀 타입과 탐색한 셀 타입이 일치하는지 체크(Cell.Type)
		if (CurrentCell->CellTypeTag.MatchesTag(CellTypeTag))
		{
			// 비어있으면서 작업이나 예약중이면 안되고 AGV의 넘버와 같아야함.
			if ((TargetCellNum == CurrentCell->CellPriority) &&
				CurrentCell->CellStateTag.MatchesTag(DFGameplayTags::Cell_State_Free) &&
				!CurrentCell->CellStateTag.MatchesTag(DFGameplayTags::Cell_State_Pending) &&
				!CurrentCell->CellStateTag.MatchesTag(DFGameplayTags::Cell_State_Working))
			{
				// 하나만 찾으면 된다.
				MoveToCell = CurrentCell;
				break;
			}
		}
	}

	// 예약 걸어두기
	if (MoveToCell && MoveToCell->CellStateTag.MatchesTag(DFGameplayTags::Cell_State_Free))
	{
		MoveToCell->SetCellStateTag(DFGameplayTags::Cell_State_Pending);
		MoveToCell->SetCellReservedAGV(AGV);

		// 블랙보드 Next좌표와 참조 셀 갱신
		BlackboardComp->SetValueAsVector(NextTargetLocationKey, MoveToCell->AGVTargetPoint->GetComponentLocation());
		BlackboardComp->SetValueAsObject(CurrentTargetCellKey, MoveToCell);

		return EBTNodeResult::Succeeded;
	}
	else
	{
		// 만약 찾지 못했을 경우 
		UE_LOG(LogTemp, Warning, TEXT("BTTask_Find : 이동할 셀을 찾지 못했습니다. 재시도 필요"));

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

FGameplayTag UBTTask_FindNextCell::GetCellTypeTagsForPhase(const FGameplayTag& AGVPhaseTag) const
{
	if (AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_Supply))
	{
		return DFGameplayTags::Cell_Type_Supply;
	}
	else if (AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_Trim))
	{
		return DFGameplayTags::Cell_Type_Trim;
	}
	else if (AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_Flexible))
	{
		return DFGameplayTags::Cell_Type_Flexible;
	}
	else if (AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_Inspection))
	{
		return DFGameplayTags::Cell_Type_Inspection;
	}
	else if (AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_Load))
	{
		return DFGameplayTags::Cell_Type_Load;
	}

	return FGameplayTag();
}
void UBTTask_FindNextCell::SetCellFree(ADFCellBase* CellToFree) const
{
	if (CellToFree && CellToFree->GetDFAbilitySystemComponent())
	{
		CellToFree->GetDFAbilitySystemComponent()->SetCellState(DFGameplayTags::Cell_State_Free);
	}
}
