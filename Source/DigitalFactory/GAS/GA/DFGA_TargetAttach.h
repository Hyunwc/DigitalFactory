// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_TargetAttach.generated.h"

class UControlRigComponent;
class AActor;
class ADFRobotArm;

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_TargetAttach : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_TargetAttach();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UControlRigComponent* ControlRigComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	AActor* TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FName EControl;

	// 타겟 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FVector TargetLocation;

	// 복귀 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FTransform ReturnTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFRobotArm* Owner;

public:
	UFUNCTION(BlueprintCallable)
	void OnAttachStart();
};
