// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AGVMoveTo.h"
#include "Robot/DFAGV.h"
#include "AIController.h"
#include "DFGameplayTags.h"

UBTTask_AGVMoveTo::UBTTask_AGVMoveTo()
{
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
	NodeName = TEXT("AGVMoveTo");
}

EBTNodeResult::Type UBTTask_AGVMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AGV)
	{
		return EBTNodeResult::Failed;
	}

	// 목적지가 Home이 아니면 이동 중으로 표시되게.
	if (!AGV->AGVPhaseTag.MatchesTag(DFGameplayTags::AGV_Phase_None))
	{
		AGV->SetAGVStateTag(DFGameplayTags::AGV_State_Move);
		//AGV->SetAGVStateTag(FGameplayTag::RequestGameplayTag("AGV.State.Return"));
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
