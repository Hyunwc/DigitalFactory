// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "DFGA_CombineTire.generated.h"

class UControlRigComponent;
class ADFRobotArm;
class ADFAGV;
class ADFVehicleBase;

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_CombineTire : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_CombineTire();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	UControlRigComponent* RigComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FName EControl;

	// 부착시킬 차량의 소켓 이름 캐시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FName SocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFRobotArm* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFAGV* TargetAGV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFVehicleBase* TargetVehicle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	FVector TargetLocation;

public:
	// 이동했으니 컨트롤 위치를 실제로 변경시키라고 콜백을 받을 함수
	UFUNCTION(BlueprintCallable)
	void OnEndMoveFinished(FTransform NewTransform);
};
