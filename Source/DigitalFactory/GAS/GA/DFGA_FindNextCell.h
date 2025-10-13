// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "DFGA_FindNextCell.generated.h"

class ADFCellBase;

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_FindNextCell : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_FindNextCell();
	
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
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
