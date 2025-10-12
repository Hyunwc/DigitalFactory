// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "BTService_FindNextTargetCell.generated.h"

class ADFCellBase;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UBTService_FindNextTargetCell : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_FindNextTargetCell();

protected:
	// Service가 실행되는동안 주기적으로 호출
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// AGV의 현재 작업 단계를 저장하는 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName CurrentAGVPhaseKey;

	// 다음에 이동할 목표 위치를 저장할 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FName NextTargetLocationKey;

	// 다음에 목표를 삼을 Cell 액터를 저장할 블랙보드 (필요할까 싶지만 일단 넣고)
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowedClasses = "DFCellBase"))
	FName CurrentTargetCellKey;

public:
	// 각 AGV Phase에 필요한 Cell Type을 매핑
	FGameplayTag GetCellTypeTagsForPhase(const FGameplayTag& AGVPhaseTag) const;

	// Cell을 Free로 변경하는 함수(AGV가 예약을 취소할 때 사용)
	void SetCellFree(ADFCellBase* CellToFree) const;
};
