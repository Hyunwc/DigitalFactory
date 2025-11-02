// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_ResetAGV.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTTask_ResetAGV : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ResetAGV();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
