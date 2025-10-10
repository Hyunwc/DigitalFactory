// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "Cell/DFCellBase.h"
#include "AbilitySystemComponent.h"

UDFGA_SimpleTimerWork::UDFGA_SimpleTimerWork()
{
	WorkDuration = 3.0f;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; // 액터당 인스턴스 하나

	WorkStartingTagsToAdd.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Working"));
	WorkStartingTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	WorkStartingTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));

	WorkEndingTagsToAdd.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	WorkEndingTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Working"));
}

void UDFGA_SimpleTimerWork::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 어빌리티 활성화 전 커밋 (비용/쿨다운 체크, 실패 시 바로 종료)
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
}

void UDFGA_SimpleTimerWork::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
}

void UDFGA_SimpleTimerWork::OnWorkCompleted()
{
}
