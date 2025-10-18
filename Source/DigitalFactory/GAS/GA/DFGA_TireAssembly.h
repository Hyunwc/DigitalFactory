// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_TireAssembly.generated.h"

class ADFCellBase;
class ADFAGV;
class AActor;
class UControlRigComponent;

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

public:
	// 어빌리티가 발동된 셀 참조 (작업 완료 후 셀에게 알리기 위함)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	ADFCellBase* OwningCell;

	// 차체를 부착시킬 AGV 참조.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFAGV* TargetAGV;

	// 감지 또는 부착한 타겟 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car")
	AActor* TargetActor;

	// 오너의 복귀 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car")
	FRotator HomeRotation;

	// 어떤 태스크에서도 사용할 컨트롤릭 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	UControlRigComponent* ControlRigComponent;

	// 이펙터의 트랜스폼 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car")
	FTransform EffectorSaveTransform;
};
