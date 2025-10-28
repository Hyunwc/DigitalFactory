// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SmoothMove.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTTask_SmoothMove : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SmoothMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

public:
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FVector CurrentLocation;

	UPROPERTY()
	float Delta;

	UPROPERTY()
	float Duration;
};
