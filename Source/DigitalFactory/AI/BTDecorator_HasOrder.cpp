// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_HasOrder.h"
#include "AIController.h"
#include "Robot/DFAGV.h"

UBTDecorator_HasOrder::UBTDecorator_HasOrder()
{
	NodeName = TEXT("Has Order(Not Idle!)");
}

bool UBTDecorator_HasOrder::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AGV)
	{
		return false;
	}

	//if (AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.None")))
	//{
	//	return false;
	//}
	//
	//return true;

	// 목적지가 None이 아니라는 것은 주문을 받아서 목적지가 달라졌다는 뜻.
	return !AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.None")) && 
		!AGV->AGVStateTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.State.Return"));
}
