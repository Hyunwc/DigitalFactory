// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetAGVPhase.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTTask_SetAGVPhase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetAGVPhase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// AGV의 현재 작업 단계를 저장할 블랙보드 키의 이름
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName AGVPhaseKeyName;

	// 블랙보드에 새로 설정할 AGV Phase의 GameplayTag
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName NewAGVPhaseTagName;
	
};
