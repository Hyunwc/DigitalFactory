// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "DFGA_RobotArmMasterAbility.generated.h"

UENUM(BlueprintType)
enum class ERobotArmPhase : uint8
{
	Idle,
	FindRotate,
	TargetAttach,
	ReturnToHome,
	CombineTire,
	Finished,
};

class ADFRobotArm;

DECLARE_DYNAMIC_MULITCAST_DELEGATE_TwoParams(FOnFinishTireAssemblyDelegate, FGameplayTag, OwnerTag, bool, bFinished);

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_RobotArmMasterAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_RobotArmMasterAbility();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:

	// 현재 단계
	UPROPERTY(VisibleAnywhere, Category = "GAS")
	ERobotArmPhase CurrentPhase;

	UPROPERTY(VisibleAnywhere, Category = "GAS")
	ADFRobotArm* Owner;

	UPROPERTY(BlueprintAssignable)
	FOnFinishTireAssemblyDelegate OnFinishTireAssembly;

public:
	// 다음 어빌리티를 시작
	UFUNCTION()
	void NextAbility();
	
	UFUNCTION()
	void OnAbilityEnded();
};
