// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_FindAndRotate.generated.h"

class UControlRigComponent;
class AActor;
class ADFRobotArm;

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_FindAndRotate : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDFGA_FindAndRotate();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Control Rig")
	UControlRigComponent* ControlRigComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car")
	AActor* TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFRobotArm* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FTransform EndTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FName AControl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FRotator StartRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FRotator TargetRot;
public:
	UFUNCTION(BlueprintCallable)
	void OnRotationFinished(FRotator FinalRotation);
};
