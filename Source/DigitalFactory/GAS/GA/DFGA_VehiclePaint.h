// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_VehiclePaint.generated.h"

class ADFCellBase;
class ADFAGV;
class UMaterialInstance;

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_VehiclePaint : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDFGA_VehiclePaint();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 종료 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 어빌리티가 발동된 셀 참조 (작업 완료 후 셀에게 알리기 위함)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	ADFCellBase* OwningCell;

	// 페인트칠할 AGV 참조.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFAGV* TargetAGV;

	// 차량에게 입힐 머티리얼 인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	UMaterialInstance* Paint;
};
