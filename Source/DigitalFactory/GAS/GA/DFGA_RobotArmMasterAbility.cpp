// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_RobotArmMasterAbility.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Robot/DFRobotArm.h"
#include "GAS/GA/DFGA_SubAbilityBase.h"

UDFGA_RobotArmMasterAbility::UDFGA_RobotArmMasterAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentPhase = ERobotArmPhase::Idle;
}

void UDFGA_RobotArmMasterAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Owner = Cast<ADFRobotArm>(ActorInfo->AvatarActor.Get());

	if (!Owner)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Owner->ActiveMaster = this; 

	UE_LOG(LogTemp, Log, TEXT("마스터 : 드디어 발동 시작!"));

	CurrentPhase = ERobotArmPhase::FindRotate;
	NextAbility(); // 첫 번째 어빌리티 시작
}

void UDFGA_RobotArmMasterAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (Owner && Owner->ActiveMaster == this)
	{
		Owner->ActiveMaster = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDFGA_RobotArmMasterAbility::NextAbility()
{
	UDFAbilitySystemComponent* DFASC = Owner->GetDFAbilitySystemComponent();
	if (!DFASC)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (CurrentPhase == ERobotArmPhase::Finished)
	{
		// 복귀한번 하고 끝
		DFASC->TryActivateAbility(Owner->ReturnToHomeSpecHandle);
		//K2_ActivateAbility();
		//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		//return;
	}

	FGameplayAbilitySpecHandle TargetHandle;
	
	switch (CurrentPhase)
	{
	case ERobotArmPhase::FindRotate:
		TargetHandle = Owner->FindSpecHandle;
		break;
	case ERobotArmPhase::TargetAttach:
		TargetHandle = Owner->TargetAttachSpecHandle;
		break;
	case ERobotArmPhase::ReturnToHome:
		TargetHandle = Owner->ReturnToHomeSpecHandle;
		break;
	case ERobotArmPhase::CombineTire:
		TargetHandle = Owner->CombineSpecHandle;
		break;
	default:
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (!TargetHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("마스터 : 어빌리티 핸들 invalid"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	bool bActivated = DFASC->TryActivateAbility(TargetHandle);

	if (!bActivated)
	{
		UE_LOG(LogTemp, Warning, TEXT("마스터 : 서브 활성화 실패"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("마스터 : 서브 활성화 성공"));
	}
}

void UDFGA_RobotArmMasterAbility::OnAbilityEnded()
{
	UE_LOG(LogTemp, Log, TEXT("마스터어빌리티 : 여기 들어오는건가"));

	if (CurrentPhase == ERobotArmPhase::Finished)
	{
		// 여기서 완전히 종료
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (CurrentPhase == ERobotArmPhase::CombineTire)
	{
		if (Owner->PhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.CombinePhase.Front")))
		{
			CurrentPhase = ERobotArmPhase::FindRotate;
			Owner->PhaseTag = FGameplayTag::RequestGameplayTag("RobotArm.CombinePhase.Back");
		}
		else
		{
			// 작업 완료한 상태
			CurrentPhase = ERobotArmPhase::Finished;
			Owner->PhaseTag = FGameplayTag::RequestGameplayTag("RobotArm.CombinePhase.Front");
		}
	}
	else
	{
		CurrentPhase = static_cast<ERobotArmPhase>(static_cast<uint8>(CurrentPhase) + 1);
	}

	UE_LOG(LogTemp, Log, TEXT("마스터어빌리티 : 끝났으니 다음 어빌리티 발동해보자"));
	NextAbility();
	//FTimerHandle DelayHandle;
	//Owner->GetWorld()->GetTimerManager().SetTimer(DelayHandle, [this]()
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("다음 어빌리티 발동 (딜레이 후)"));
	//		NextAbility();
	//	}, 0.1f, false);
}

