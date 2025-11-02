// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "BTDecorator_IsWorking.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTDecorator_IsWorking : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsWorking();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
