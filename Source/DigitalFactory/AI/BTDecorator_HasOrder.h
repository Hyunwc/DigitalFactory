// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasOrder.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTDecorator_HasOrder : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_HasOrder();

public:
	/* 데코레이터 조건 체크 함수
	* AGV의 현재 상태가 Idle이 아닌지 파악한다.
	*/
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
