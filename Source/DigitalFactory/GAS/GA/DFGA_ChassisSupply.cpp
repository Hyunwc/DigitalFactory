// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_ChassisSupply.h"
#include "Cell/DFCellBase.h"
#include "Vehicle/DFVehicleBase.h"
#include "Robot/DFAGV.h"
#include "Engine/TargetPoint.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UDFGA_ChassisSupply::UDFGA_ChassisSupply()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; // 액터당 인스턴스 하나
	SocketName = TEXT("AttachSocket");
}

void UDFGA_ChassisSupply::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 블루프린트에서 정의된 이벤트 호출
	K2_ActivateAbility();
}

void UDFGA_ChassisSupply::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsActive())
	{
		return;
	}

	if (OwningCell && OwningCell->GetDFAbilitySystemComponent())
	{
		UE_LOG(LogTemp, Log, TEXT("GA_Chassis : 참조중인 셀 이름 %s"), *OwningCell->GetName());
		OwningCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));

		UE_LOG(LogTemp, Log, TEXT("GA_Chassis : 차체 부착 작업 끝났어요!"));
		// 작업이 끝났음을 알림
		OwningCell->OnCellWorkComplete.Broadcast(OwningCell);
	}
	
	// 참조중인 데이터들은 초기화
	//OwningCell = nullptr;
	//TargetAGV = nullptr;
	//SupplyVehicle = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


