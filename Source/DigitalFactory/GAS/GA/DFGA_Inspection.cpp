// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_Inspection.h"
#include "Cell/DFCellBase.h"
#include "Robot/DFAGV.h"
#include "Vehicle/DFVehicleBase.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "GameFramework/Actor.h"

UDFGA_Inspection::UDFGA_Inspection()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDFGA_Inspection::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		UE_LOG(LogTemp, Log, TEXT("GA_Inspection : 검차 작업 시작할게요!!"));

		// 일단 필요할수도 있으니 변수로 저장
		TargetAGV = OwningCell->WorkingAGV;
		TargetVehicle = TargetAGV->AttachVehicle;
	}

	// 바퀴 결합상태 탐색
	for (const auto& el : TargetVehicle->TireCombineMap)
	{
		if (el.Value == false)
		{
			UE_LOG(LogTemp, Log, TEXT("GA_Inspection : 앗 타이어 결합이 잘못됐는데요?"));
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("GA_Inspection : 문제 없네요~"));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	TargetVehicle->Destroy();
	TargetAGV->AttachVehicle = nullptr;
}

void UDFGA_Inspection::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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
