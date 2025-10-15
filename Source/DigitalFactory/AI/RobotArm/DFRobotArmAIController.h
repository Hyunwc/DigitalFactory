// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DFRobotArmAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFRobotArmAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADFRobotArmAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;
	
public:
	void RunAI();
	
	void StopAI();

private:
	UPROPERTY()
	UBlackboardData* BBAsset;

	UPROPERTY()
	UBehaviorTree* BTAsset;
};
