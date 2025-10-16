// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_TireAssembly.generated.h"

class ADFCellBase;
class ADFAGV;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_TireAssembly : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_TireAssembly();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 어빌리티가 발동된 셀 참조 (작업 완료 후 셀에게 알리기 위함)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	ADFCellBase* OwningCell;

	// 차체를 부착시킬 AGV 참조.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFAGV* TargetAGV;
};
