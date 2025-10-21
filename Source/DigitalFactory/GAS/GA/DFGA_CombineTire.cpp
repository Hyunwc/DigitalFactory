// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_CombineTire.h"
#include "ControlRigComponent.h"
#include "Robot/DFRobotArm.h"
#include "Robot/DFAGV.h"
#include "Vehicle/DFVehicleBase.h"
#include "Vehicle/DFTire.h"

UDFGA_CombineTire::UDFGA_CombineTire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	EControl = FName("Robot1_End_ctrl");
}

void UDFGA_CombineTire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	Owner = Cast<ADFRobotArm>(ActorInfo->AvatarActor);
	if (!Owner)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	RigComp = Owner->ControlRigComponent;
	TargetAGV = Owner->TargetAGV;
	TargetVehicle = Owner->TargetAGV->AttachVehicle;
	
	//if (CurrentAGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle")))
	// 소켓 탐색
	if (Owner->PhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.CombinePhase.Front")))
	{
		if (Owner->TypeTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.Type.Left")))
		{
			TargetLocation = TargetVehicle->Chassis->GetSocketLocation("LeftFrontSocket");
			SocketName = FName("LeftFrontSocket");
			SocketTagKey = FGameplayTag::RequestGameplayTag("Vehicle.Tire.Front.Left");
		}
		else if (Owner->TypeTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.Type.Right")))
		{
			TargetLocation = TargetVehicle->Chassis->GetSocketLocation("RightFrontSocket");
			SocketName = FName("RightFrontSocket");
			SocketTagKey = FGameplayTag::RequestGameplayTag("Vehicle.Tire.Front.Right");
		}
	}
	else if (Owner->PhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.CombinePhase.Back")))
	{
		if (Owner->TypeTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.Type.Left")))
		{
			TargetLocation = TargetVehicle->Chassis->GetSocketLocation("LeftBackSocket");
			SocketName = FName("LeftBackSocket");
			SocketTagKey = FGameplayTag::RequestGameplayTag("Vehicle.Tire.Back.Left");
		}
		else if (Owner->TypeTag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.Type.Right")))
		{
			TargetLocation = TargetVehicle->Chassis->GetSocketLocation("RightBackSocket");
			SocketName = FName("RightBackSocket");
			SocketTagKey = FGameplayTag::RequestGameplayTag("Vehicle.Tire.Back.Right");
		}
	}

	// 블루프린트에서 보간이동 진행
	K2_ActivateAbility();
}

void UDFGA_CombineTire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDFGA_CombineTire::OnEndMoveFinished(FTransform NewTransform)
{
	RigComp->SetControlTransform(EControl, NewTransform, EControlRigComponentSpace::WorldSpace);

	if (TargetVehicle && Owner)
	{
		if (Owner->TargetActor->AttachToComponent(TargetVehicle->Chassis,
			FAttachmentTransformRules::KeepWorldTransform, SocketName))
		{
			ADFTire* Tire = Cast<ADFTire>(Owner->TargetActor);
			Tire->bCombined = true;
			TargetVehicle->TireCombineMap.FindOrAdd(SocketTagKey) = true;
			UE_LOG(LogTemp, Log, TEXT("GA_Combine : 부착했어!!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("GA_Combine : 부착실패!!"));
		}
	}

	if (Owner)
	{
		Owner->NotifySubAbilityFinished();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
