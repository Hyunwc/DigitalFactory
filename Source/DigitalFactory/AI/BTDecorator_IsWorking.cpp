// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsWorking.h"
#include "AIController.h"
#include "Robot/DFAGV.h"

UBTDecorator_IsWorking::UBTDecorator_IsWorking()
{
	NodeName = TEXT("IsWorking?");
}

bool UBTDecorator_IsWorking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AGV)
	{
		return false;
	}

	// 복귀 상태가 아니면 사이클 계속 돌 것.
	return AGV->AGVStateTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.State.Return"));
}
