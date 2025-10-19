// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DFGA_SubAbilityBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedSubAbility);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFGA_SubAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDFGA_SubAbilityBase();

public:
	UPROPERTY(BlueprintAssignable)
	FOnFinishedSubAbility OnFinishedSubAbility;

//protected:
//	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
