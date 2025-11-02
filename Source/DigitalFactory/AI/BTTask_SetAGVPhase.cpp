// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetAGVPhase.h"
#include "DFAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Robot/DFAGV.h"
#include "AIController.h"

UBTTask_SetAGVPhase::UBTTask_SetAGVPhase()
{
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
	NodeName = TEXT("Set AGV Phase");
	AGVPhaseKeyName = BBKEY_CURRENTPHASE;
}

EBTNodeResult::Type UBTTask_SetAGVPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드의 AGV Phase 키 값을 새로운 Phase 태그 이름으로 업데이트
	//BlackboardComp->SetValueAsName(AGVPhaseKeyName, NewAGVPhaseTagName);

	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	
	// Phase를 설정.
	AGV->SetAGVPhaseTag(GetNewPhase(AGV->AGVPhaseTag));

	// 만약 Home이면 State는 복귀 상태로
	if (AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Home")))
	{
		AGV->SetAGVStateTag(FGameplayTag::RequestGameplayTag("AGV.State.Return"));
	}
	// 복귀중이였을 때 로직
	//if (AGV->AGVStateTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.State.Return")))
	//{
	//	//AGV->SetAGVPhaseTag(FGameplayTag::RequestGameplayTag("AGV.Phase.None"));
	//	AGV->SetAGVStateTag(FGameplayTag::RequestGameplayTag("AGV.State.Idle"));
	//	return EBTNodeResult::Succeeded;
	//}

	return EBTNodeResult::Succeeded;
}

FGameplayTag UBTTask_SetAGVPhase::GetNewPhase(FGameplayTag CurrentPhaseTag)
{	
	// Supply -> Flexible -> Trim -> Inspection -> Load -> Return
	if (CurrentPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Supply")))
	{
		return FGameplayTag::RequestGameplayTag("AGV.Phase.Flexible");
	}
	else if (CurrentPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Flexible")))
	{
		return FGameplayTag::RequestGameplayTag("AGV.Phase.Trim");
	}
	else if (CurrentPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Trim")))
	{
		return FGameplayTag::RequestGameplayTag("AGV.Phase.Inspection");
	}
	else if (CurrentPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Inspection")))
	{
		return FGameplayTag::RequestGameplayTag("AGV.Phase.Load");
	}
	else if (CurrentPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Load")))
	{
		return FGameplayTag::RequestGameplayTag("AGV.Phase.Home");
	}

	return FGameplayTag();
}
