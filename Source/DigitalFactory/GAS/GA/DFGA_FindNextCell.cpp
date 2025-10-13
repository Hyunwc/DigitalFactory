// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_FindNextCell.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/DFAI.h"
#include "Robot/DFAGV.h"
#include "AIController.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Cell/DFCellBase.h"
#include "EngineUtils.h"

UDFGA_FindNextCell::UDFGA_FindNextCell()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; // 액터당 인스턴스 하나

	CurrentAGVPhaseKey = BBKEY_CURRENTPHASE;
	NextTargetLocationKey = BBKEY_NEXTPOS;
	CurrentTargetCellKey = BBKEY_TARGETCELL;
}

void UDFGA_FindNextCell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// AvatarActor는 어빌리티를 발동시킨 AGV
	ADFAGV* AGV = Cast<ADFAGV>(ActorInfo->AvatarActor.Get());
	AAIController* AGVAICon = AGV ? Cast<AAIController>(AGV->GetController()) : nullptr;
	UBlackboardComponent* BlackboardComp = AGVAICon ? AGVAICon->GetBlackboardComponent() : nullptr;

	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Error, TEXT("UDFGA_FindNextCell : 블랙보드를 가져오질 못해서 작업 종료할게!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// AGV의 현재 작업 단계(AGV.Phase)를 블랙보드에서 가져온다.
	FGameplayTag CurrentAGVPhaseTag = FGameplayTag::RequestGameplayTag(BlackboardComp->GetValueAsName(CurrentAGVPhaseKey));
	if (!CurrentAGVPhaseTag.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// AGV가 가야할 셀 타입 검출
	FGameplayTag CellTypeTag = GetCellTypeTagsForPhase(CurrentAGVPhaseTag);

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
		UE_LOG(LogTemp, Log, TEXT("GA_FindNextCell : 예약 걸어둘게요!"));
		UDFAbilitySystemComponent* CellASC = MoveToCell->GetDFAbilitySystemComponent();
		if (CellASC->HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")))
		{
			CellASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
			CellASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));
		}

		// 블랙보드 Next좌표와 참조 셀 갱신
		BlackboardComp->SetValueAsVector(NextTargetLocationKey, MoveToCell->AGVTargetPoint->GetComponentLocation());
		BlackboardComp->SetValueAsObject(CurrentTargetCellKey, MoveToCell);
	}
}

void UDFGA_FindNextCell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 이미 종료되었으면 다시 처리 x
	if (!IsActive())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("GA_FindNextCell : 찾았으니 탐색 어빌리티 종료합니다!"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FGameplayTag UDFGA_FindNextCell::GetCellTypeTagsForPhase(const FGameplayTag& AGVPhaseTag) const
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

void UDFGA_FindNextCell::SetCellFree(ADFCellBase* CellToFree) const
{
	if (CellToFree && CellToFree->GetDFAbilitySystemComponent())
	{
		CellToFree->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	}
}
