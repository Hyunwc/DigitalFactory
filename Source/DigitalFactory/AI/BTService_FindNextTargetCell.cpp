// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNextTargetCell.h"
#include "AI/DFAI.h"
#include "Cell/DFCellBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "GAS/DFAbilitySystemComponent.h"


UBTService_FindNextTargetCell::UBTService_FindNextTargetCell()
{
	NodeName = TEXT("Find Next Target Cell");
	// Tick 주기 설정
	Interval = 2.0f;
	//RandomDeviation = 0.1f;

	// 블랙보드 키의 기본값 설정
	//CurrentAGVPhaseKey.AddNameFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindNextTargetCell, CurrentAGVPhaseKey));
	//NextTargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindNextTargetCell, NextTargetLocationKey));
	//CurrentTargetCellKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindNextTargetCell, CurrentTargetCellKey), ADFCellBase::StaticClass());
	
	CurrentAGVPhaseKey = BBKEY_CURRENTPHASE;
	NextTargetLocationKey = BBKEY_NEXTPOS;
	CurrentTargetCellKey = BBKEY_TARGETCELL;
}

void UBTService_FindNextTargetCell::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return;
	}
	
	// AGV의 현재 작업 단계(Phase)를 블랙보드에서 가져온다
	FName CurrentAGVPhaseName = BlackboardComp->GetValueAsName(CurrentAGVPhaseKey);
	FGameplayTag CurrentAGVPhaseTag = FGameplayTag::RequestGameplayTag(CurrentAGVPhaseName);

	//UE_LOG(LogTemp, Log, TEXT("BTService : 현재 태그는 %s입니다"), *CurrentAGVPhaseName.ToString());
	if (!CurrentAGVPhaseTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("BTService_Find : Tag Not Valid"));
		return;
	}

	// 대기중일땐
	//if (CurrentAGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle")))
	//{
	//	BlackboardComp->ClearValue(NextTargetLocationKey);
	//	BlackboardComp->ClearValue(CurrentTargetCellKey);
	//
	//	ADFCellBase* PrevCell = Cast<ADFCellBase>(BlackboardComp->GetValueAsObject(CurrentTargetCellKey));
	//	if (PrevCell && PrevCell->GetAbilitySystemComponent() &&
	//		PrevCell->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending")))
	//	{
	//		SetCellFree(PrevCell);
	//	}
	//
	//	return;
	//}
	//else if(CurrentAGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle")))

	// 셀 Type 검출
	FGameplayTag RequiredCellTypeTag = GetCellTypeTagsForPhase(CurrentAGVPhaseTag);
	if (!RequiredCellTypeTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("BTService_Find : Tag Not Valid"));
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("BTService : Require의 태그는 %s입니다"), *RequiredCellTypeTag.GetTagName().ToString());

	ADFCellBase* BestCell = nullptr;

	// 이전 예약 셀이 유효한지 확인하고 계속 목표로 삼을지 결정
	ADFCellBase* PrevPendingCell = Cast<ADFCellBase>(BlackboardComp->GetValueAsObject(CurrentTargetCellKey));
	//UE_LOG(LogTemp, Log, TEXT("BTService : 너의 정체는 뭐니? %s"), *PrevPendingCell->GetName());

	if (IsValid(PrevPendingCell))
	{
		UE_LOG(LogTemp, Log, TEXT("BTService : PrevPending은 유효해!"));
		if (PrevPendingCell->CellTypeTag.MatchesTag(RequiredCellTypeTag))
		{
			UE_LOG(LogTemp, Log, TEXT("BTService : PrevPending의 태그는 %s"), *PrevPendingCell->GetDFAbilitySystemComponent()->GetCurrentCellStateTag().GetTagName().ToString());
			if(IsValid(PrevPendingCell->GetDFAbilitySystemComponent()) &&
				PrevPendingCell->GetDFAbilitySystemComponent()->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending")) &&
					!PrevPendingCell->GetDFAbilitySystemComponent()->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
			{
				BestCell = PrevPendingCell;
				GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Cyan, *BestCell->GetName());
				UE_LOG(LogTemp, Log, TEXT("BTService : BestCell 이름 %s"), *BestCell->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("BTService : BestCell를 못찾겠어요"));
				if (IsValid(PrevPendingCell->GetDFAbilitySystemComponent())
					&& PrevPendingCell->GetDFAbilitySystemComponent()->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending")))
				{
					SetCellFree(PrevPendingCell);
				}
			}
		}
	}
	
	if(!BestCell)
	{
		for (TActorIterator<ADFCellBase> It(GetWorld()); It; ++It)
		{
			ADFCellBase* CurrentCell = *It;
			if (CurrentCell && CurrentCell->GetDFAbilitySystemComponent())
			{
				UDFAbilitySystemComponent* CellASC = CurrentCell->GetDFAbilitySystemComponent();

				if (CurrentCell->CellTypeTag.MatchesTag(RequiredCellTypeTag) &&
					CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")) &&
					!CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending")) &&
					!CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
				{
					BestCell = CurrentCell;
					UE_LOG(LogTemp, Log, TEXT("BTService : 새로운 베스트를 찾았다! 그 이름은 %s"), *BestCell->GetName());
					break;
				}
			}
		}

		if (BestCell)
		{
			UDFAbilitySystemComponent* BestCellASC = BestCell->GetDFAbilitySystemComponent();
			if (BestCellASC && BestCellASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Free")))
			{
				BestCellASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
				BestCellASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));
			}
		}
	}

	if (BestCell)
	{
		BlackboardComp->SetValueAsVector(NextTargetLocationKey, BestCell->AGVTargetPoint->GetComponentLocation());
		BlackboardComp->SetValueAsObject(CurrentTargetCellKey, BestCell);
	}
}

FGameplayTag UBTService_FindNextTargetCell::GetCellTypeTagsForPhase(const FGameplayTag& AGVPhaseTag) const
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

void UBTService_FindNextTargetCell::SetCellFree(ADFCellBase* CellToFree) const
{
	if (CellToFree && CellToFree->GetDFAbilitySystemComponent())
	{
		UDFAbilitySystemComponent* CellASC = CellToFree->GetDFAbilitySystemComponent();
		// 예약해제
		CellASC->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	}
}
