// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "DFGA_SimpleTimerWork.generated.h"

class ADFCellBase;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_SimpleTimerWork : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDFGA_SimpleTimerWork();
	
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	// 작업 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	float WorkDuration;

	// 작업 타이머 핸들
	FTimerHandle WorkTimerHandle;

	// 어빌리티 활성화 시 셀의 상태 태그 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FGameplayTagContainer WorkStartingTagsToAdd; // 작업 시작 시 부여할 태그 (Cell.State.Working)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FGameplayTagContainer WorkStartingTagsToRemove; // 작업 시작 시 제거할 태그 (Cell.State.Free, Pending)

	// 어빌리티 종료 시 셀의 상태 태그 변경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FGameplayTagContainer WorkEndingTagsToAdd; // 작업 종료 시 부여할 태그 (Cell.State.Free)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Work")
	FGameplayTagContainer WorkEndingTagsToRemove; // 작업 종료 시 제거할 태그 (Cell.State.Working)

	// 어빌리티가 발동된 셀 참조 (작업 완료 후 셀에게 알리기 위함)
	UPROPERTY()
	ADFCellBase* OwningCell;

public:
	// 작업 완료시 호출될 함수
	UFUNCTION()
	void OnWorkCompleted();
};
