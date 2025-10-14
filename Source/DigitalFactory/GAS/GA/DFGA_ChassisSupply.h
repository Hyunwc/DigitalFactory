// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_ChassisSupply.generated.h"

class ADFCellBase;
class ADFVehicleBase;
class ATargetPoint;
class ADFAGV;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_ChassisSupply : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_ChassisSupply();

public:
	// 어빌리티 활성화 시 호출되는 함수 (작업 시작)
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// 어빌리티 취소 시 호출되는 함수 (작업 중단)
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// 어빌리티가 발동된 셀 참조 (작업 완료 후 셀에게 알리기 위함)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	ADFCellBase* OwningCell;

	// 스폰시킬 차체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Car")
	TSubclassOf<ADFVehicleBase> VehicleClass;

	// 스폰한 차체
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFVehicleBase* SupplyVehicle;

	// 스폰지점
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	FVector SpawnPoint;

	// 차체를 부착시킬 AGV 참조.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFAGV* TargetAGV;

	// AGV에서 검사할 소켓이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Car")
	FName SocketName;

};
