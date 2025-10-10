// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitCellWork.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTTask_WaitCellWork : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_WaitCellWork();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float ElapsedTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float WaitDuration;
};
