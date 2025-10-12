// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitCellWork.generated.h"

// 블랙보드에 저장된 현재 목표 셀의 델리게이트를 구독하고 브로드캐스트되면 작업을 성공적으로 간주하여 
// Succeeded 반환

class ADFCellBase;
class UBehaviourTreeComponent;
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

	// Task가 중단되거나 완료될 때 호출
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

public:
	// 블랙보드에서 현재 작업 중인 Cell 액터 레퍼런스가 저장된 키의 이름
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName TargetCellKeyName;

	// 현재 작업 중인 셀 참조
	UPROPERTY()
	ADFCellBase* CurrentWorkingCell;

	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompRef;

public:
	// 셀의 작업 완료 델리게이트에 바인딩될 함수
	UFUNCTION()
	void OnCellWorkCompleted(ADFCellBase* CompletedCell);
};
