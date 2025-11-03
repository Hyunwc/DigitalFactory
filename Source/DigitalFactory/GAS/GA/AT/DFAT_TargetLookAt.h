// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DFAT_TargetLookAt.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFAT_TargetLookAt : public UAbilityTask
{
	GENERATED_BODY()

public:
	UDFAT_TargetLookAt(const FObjectInitializer& ObjectInitializer);

	// 액터를 타겟을 바라보게끔
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "StartLookAt", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UDFAT_TargetLookAt* StartLookAt(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		FVector TargetLocation,
		UStaticMeshComponent* Actor,
		float Duration
	);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

public:
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	UStaticMeshComponent* Looker;

	UPROPERTY()
	float Duration = 0.0f;

	UPROPERTY()
	float ElapsedTime = 0.0f;
};
