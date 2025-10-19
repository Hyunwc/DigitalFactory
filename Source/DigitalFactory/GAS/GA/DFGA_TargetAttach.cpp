// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_TargetAttach.h"
#include "ControlRigComponent.h"
#include "GameFramework/Actor.h"
#include "Robot/DFRobotArm.h"

UDFGA_TargetAttach::UDFGA_TargetAttach()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	EControl = FName("Robot1_End_ctrl");
}

void UDFGA_TargetAttach::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 소유자(로봇암) 저장
	Owner = Cast<ADFRobotArm>(ActorInfo->AvatarActor);
	UE_LOG(LogTemp, Log, TEXT("GA_Find : 오너의 이름 %s"), *Owner->GetName());

	if (!Owner)
	{
		UE_LOG(LogTemp, Log, TEXT("GA_Find : 오너가 유효하지 않습니다"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 컨트롤릭 탐색
	ControlRigComponent = Owner->ControlRigComponent;
	TargetActor = Owner->TargetActor;
	TargetLocation = TargetActor->GetActorLocation();
	UE_LOG(LogTemp, Log, TEXT("GA_Target : 타겟의 위치 X : %f, Y : %f, Z : %f"), TargetActor->GetActorLocation().X,
		TargetActor->GetActorLocation().Y, TargetActor->GetActorLocation().Z);

	K2_ActivateAbility();
}

void UDFGA_TargetAttach::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDFGA_TargetAttach::OnAttachStart()
{
	// TODO : 어태치 작업 코드 작성
	UE_LOG(LogTemp, Log, TEXT("GA_Attach : 고마워 어태치 시작해볼게"));

	if (Owner)
	{
		Owner->NotifySubAbilityFinished();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
