// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_TireAssembly.h"
#include "Cell/DFCellBase.h"
#include "Robot/DFAGV.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "ControlRigComponent.h"

UDFGA_TireAssembly::UDFGA_TireAssembly()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	HomeRotation = FRotator::ZeroRotator;
}

void UDFGA_TireAssembly::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	//OwningCell = ActorInfo->OwnerActor.Get()

	K2_ActivateAbility();
}

void UDFGA_TireAssembly::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsActive())
	{
		return;
	}

	if (OwningCell && OwningCell->GetDFAbilitySystemComponent())
	{
		UE_LOG(LogTemp, Log, TEXT("GA_Tire : 참조중인 셀 이름 %s"), *OwningCell->GetName());
		OwningCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));

		UE_LOG(LogTemp, Log, TEXT("GA_Tire : 타이어 결합 작업 끝났어요!"));
		// 작업이 끝났음을 알림
		OwningCell->OnCellWorkComplete.Broadcast(OwningCell);
	}

	// 참조중인 데이터들은 초기화
	//OwningCell = nullptr;
	//TargetAGV = nullptr;
	//SupplyVehicle = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
