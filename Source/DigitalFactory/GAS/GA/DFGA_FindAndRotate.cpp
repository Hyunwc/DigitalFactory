// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_FindAndRotate.h"
#include "ControlRigComponent.h"
#include "GameFramework/Actor.h"
#include "Robot/DFRobotArm.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GAS/GA/AT/DFAT_SoftRotate.h"

UDFGA_FindAndRotate::UDFGA_FindAndRotate()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AControl = FName("Robot1_A_ctrl");
}

void UDFGA_FindAndRotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
	ControlRigComponent = Owner->GetComponentByClass<UControlRigComponent>();

	// 타겟 탐색
	USphereComponent* SearchSphere = Owner->FindComponentByClass<USphereComponent>();
	if (!SearchSphere)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	TArray<AActor*> OverlappingActors;
	SearchSphere->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() == 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	TargetActor = OverlappingActors[0];
	Owner->TargetActor = TargetActor;
	TargetLocation = TargetActor->GetActorLocation();

	FName EControl = TEXT("Robot1_End_ctrl");
	FTransform ATrs = ControlRigComponent->GetControlTransform(AControl, EControlRigComponentSpace::WorldSpace);
	FTransform ETrs = ControlRigComponent->GetControlTransform(EControl, EControlRigComponentSpace::WorldSpace);

	// 회전 계산
	StartRot = ATrs.GetRotation().Rotator();
	FVector EndLoc = ETrs.GetLocation();
	// End가 타겟을 바라보게끔
	FRotator LookAt_End = UKismetMathLibrary::FindLookAtRotation(EndLoc, TargetLocation);
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(LookAt_End, StartRot);
	TargetRot = FRotator(StartRot.Pitch, LookAt_End.Yaw + Delta.Yaw, StartRot.Roll);

	// End
	FVector LocalOffset = ATrs.InverseTransformPosition(ETrs.GetLocation());
	FTransform RotatedA(TargetRot, ATrs.GetLocation());
	EndTransform = FTransform(LookAt_End, RotatedA.TransformPosition(LocalOffset));

	//UDFAT_SoftRotate* RotateTask;
	//RotateTask->OnFinishedRotate.AddDynamic(this, &UDFGA_FindAndRotate::OnRotationFinished);
	// 나머지 로직은 블루프린트에서 처리해야함
	K2_ActivateAbility();
}

void UDFGA_FindAndRotate::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UDFGA_FindAndRotate::OnRotationFinished(FRotator FinalRotation)
{
	ControlRigComponent->SetControlRotator(AControl, FinalRotation, EControlRigComponentSpace::WorldSpace);
	ControlRigComponent->SetControlTransform(TEXT("Robot1_End_ctrl"), EndTransform, EControlRigComponentSpace::WorldSpace);
	// FK 적용 즉시 강제 업데이트
	//ControlRigComponent->GetControlRig()->Evaluate_AnyThread();

	//FTransform Cur = ControlRigComponent->GetControlTransform(TEXT("Robot1_End_ctrl"), EControlRigComponentSpace::WorldSpace);
	//UE_LOG(LogTemp, Warning, TEXT("GA_Rotate : EndTransform X : %f, Y : %f, Z : %f"),
	//	EndTransform.GetLocation().X, EndTransform.GetLocation().Y, EndTransform.GetLocation().Z);
	if (Owner)
	{
		Owner->NotifySubAbilityFinished();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
