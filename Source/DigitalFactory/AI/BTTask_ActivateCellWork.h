// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateCellWork.generated.h"

// 블랙보드에 저장된 현재 목표 셀의 StartWork 함수를 호출하여 셀의 작업을 시작시키는 Task
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTTask_ActivateCellWork : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateCellWork();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// 블랙보드에서 현재 작업 중인 Cell 액터 레퍼런스가 저장된 키의 이름
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetCellKeyName;
};
