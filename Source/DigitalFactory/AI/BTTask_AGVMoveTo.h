// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "GameplayTagContainer.h"
#include "BTTask_AGVMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTTask_AGVMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UBTTask_AGVMoveTo();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
