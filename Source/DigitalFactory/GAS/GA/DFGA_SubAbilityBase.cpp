// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_SubAbilityBase.h"

//void UDFGA_SubAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
//{
//	OnFinishedSubAbility.Broadcast();
//
//	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

UDFGA_SubAbilityBase::UDFGA_SubAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
