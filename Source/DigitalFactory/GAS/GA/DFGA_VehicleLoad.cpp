// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_VehicleLoad.h"
#include "Cell/DFCellBase.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UDFGA_VehicleLoad::UDFGA_VehicleLoad()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDFGA_VehicleLoad::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 블루프린트에서 정의된 이벤트 호출
	K2_ActivateAbility();
}

void UDFGA_VehicleLoad::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (OwningCell && OwningCell->GetDFAbilitySystemComponent())
	{
		UE_LOG(LogTemp, Log, TEXT("GA_Chassis : 참조중인 셀 이름 %s"), *OwningCell->GetName());
		OwningCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));

		UE_LOG(LogTemp, Log, TEXT("%s의 GA_Chassis : 차체 부착 작업 끝났어요!"), *OwningCell->GetName());
		// 작업이 끝났음을 알림
		OwningCell->OnCellWorkComplete.Broadcast(OwningCell);
		OwningCell->GetDFAbilitySystemComponent()->ClearAbility(CurrentSpecHandle);
	}
}
