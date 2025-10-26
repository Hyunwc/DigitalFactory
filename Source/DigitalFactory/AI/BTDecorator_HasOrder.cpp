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

	// Idle이 아닐때 true 반환
	return !AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle"));
}
