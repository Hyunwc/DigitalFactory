// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_ReturnToHome.h"
#include "ControlRigComponent.h"
#include "Robot/DFRobotArm.h"

UDFGA_ReturnToHome::UDFGA_ReturnToHome()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AControl = FName("Robot1_A_ctrl");
	EControl = FName("Robot1_End_ctrl");
}

void UDFGA_ReturnToHome::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	Owner = Cast<ADFRobotArm>(ActorInfo->AvatarActor);

	RigComp = Owner->GetComponentByClass<UControlRigComponent>();

	K2_ActivateAbility();
}

void UDFGA_ReturnToHome::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDFGA_ReturnToHome::OnEndReturnToHomeFinished(FTransform HomeTransform)
{
	RigComp->SetControlTransform(EControl, HomeTransform, EControlRigComponentSpace::WorldSpace);

	UE_LOG(LogTemp, Log, TEXT("GA_Return End : 우와 복귀 완료!"));

	UE_LOG(LogTemp, Warning, TEXT("GA_Return End : End의 최종 복귀위치 X : %f, Y : %f, Z : %f")
		, HomeTransform.GetLocation().X, HomeTransform.GetLocation().Y, HomeTransform.GetLocation().Z);

	FRotator TestR = RigComp->GetControlRotator(AControl, EControlRigComponentSpace::WorldSpace);
	UE_LOG(LogTemp, Warning, TEXT("GA_Return A : 현재 A의 방향 X : %f, Y : %f, Z : %f")
		, TestR.Pitch, TestR.Yaw, TestR.Roll);
}

void UDFGA_ReturnToHome::OnAReturnToHomeFinished(FRotator HomeRotation)
{
	RigComp->SetControlRotator(AControl, HomeRotation, EControlRigComponentSpace::WorldSpace);
	UE_LOG(LogTemp, Log, TEXT("GA_Return A : 우와 복귀 완료!"));
	UE_LOG(LogTemp, Warning, TEXT("GA_Return A : A의 최종 복귀위치 X : %f, Y : %f, Z : %f")
		, HomeRotation.Pitch, HomeRotation.Yaw, HomeRotation.Roll);
	if (Owner)
	{
		Owner->NotifySubAbilityFinished();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

