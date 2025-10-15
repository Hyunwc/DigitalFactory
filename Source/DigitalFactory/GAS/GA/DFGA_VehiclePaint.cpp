// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_VehiclePaint.h"
#include "Cell/DFCellBase.h"
#include "Robot/DFAGV.h"
#include "Materials/MaterialInstance.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UDFGA_VehiclePaint::UDFGA_VehiclePaint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDFGA_VehiclePaint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 셀 참조 
	OwningCell = Cast<ADFCellBase>(ActorInfo->OwnerActor.Get());
	if (OwningCell && OwningCell->GetDFAbilitySystemComponent())
	{
		OwningCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working"));
		UE_LOG(LogTemp, Log, TEXT("GA_Paint : 도색 작업 시작할게요!!"));
	}

	// 타겟 AGV 참조
	TargetAGV = OwningCell->WorkingAGV;

	// TODO : 블루프린트에서 로직짜기
	K2_ActivateAbility();
}

void UDFGA_VehiclePaint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 이미 종료되었으면 다시 처리 x
	if (!IsActive())
	{
		return;
	}

	// 작업 종료 태그 적용
	if (OwningCell && OwningCell->GetDFAbilitySystemComponent())
	{
		OwningCell->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));

		// 작업이 완료되었음을 델리게이트를 통해 알림
		OwningCell->OnCellWorkComplete.Broadcast(OwningCell);
	}

	//OwningCell = nullptr;
	//TargetAGV = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
