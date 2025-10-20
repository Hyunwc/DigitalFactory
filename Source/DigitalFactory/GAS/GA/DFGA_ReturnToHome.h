// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_ReturnToHome.generated.h"

class UControlRigComponent;
class ADFRobotArm;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_ReturnToHome : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_ReturnToHome();

	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	UControlRigComponent* RigComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFRobotArm* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FName AControl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FName EControl;

public:
	// 콜백
	UFUNCTION(BlueprintCallable)
	void OnEndReturnToHomeFinished(FTransform HomeTransform);

	UFUNCTION(BlueprintCallable)
	void OnAReturnToHomeFinished(FRotator HomeRotation);

};
