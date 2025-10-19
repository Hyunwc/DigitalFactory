// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_SoftRotate.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "ControlRigComponent.h"

UDFAT_SoftRotate::UDFAT_SoftRotate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UDFAT_SoftRotate* UDFAT_SoftRotate::StartSoftRotate(UGameplayAbility* OwningAbility, FName TaskInstanceName, UControlRigComponent* RigComp, FName ControlName, FRotator StartRot, FRotator TargetRot, AActor* Target, float Duration)
{
	// 객체 생성
	UDFAT_SoftRotate* MyObj = NewAbilityTask<UDFAT_SoftRotate>(OwningAbility, TaskInstanceName);

	if (MyObj)
	{
		MyObj->ControlRigComponent = RigComp;
		MyObj->TargetControlName = ControlName;
		MyObj->StartRotation = StartRot;
		MyObj->TargetRotation = TargetRot;
		MyObj->TargetActor = Target;
		MyObj->Duration = FMath::Max(Duration, 0.001f);
	}

	return MyObj;
}

void UDFAT_SoftRotate::Activate()
{
	if (!ControlRigComponent)
	{
		EndTask();
		return;
	}

	ElapsedTime = 0.0f;
	bTickingTask = true;
	CurrentTransform = ControlRigComponent->GetControlTransform(TEXT("Robot1_End_ctrl"), EControlRigComponentSpace::WorldSpace);
}

void UDFAT_SoftRotate::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	// 회전 보간
	FRotator NewRot = FMath::Lerp(StartRotation, TargetRotation, Alpha);
	ControlRigComponent->SetControlRotator(TargetControlName, NewRot, EControlRigComponentSpace::WorldSpace);

	//FName EndCtrl = TEXT("Robot1_End_ctrl");
	//FTransform ATrs = ControlRigComponent->GetControlTransform(TargetControlName, EControlRigComponentSpace::WorldSpace);
	//FTransform ETrs = ControlRigComponent->GetControlTransform(EndCtrl, EControlRigComponentSpace::WorldSpace);


	//FVector NewLocation = FMath::Lerp(CurrentTransform.GetLocation(), TargetActor->GetActorLocation(), Alpha);
	//FVector NewLocation = ETrs.GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("AT_Target : NewLocation X : %f, Y : %f, Z : %f"),
	//	NewLocation.X, NewLocation.Y, NewLocation.Z);
	//FQuat NewQuat = FQuat::Slerp(CurrentTransform.GetRotation(), TargetActor->GetActorQuat(), Alpha).GetNormalized();
	//
	//FTransform NewTransform(NewQuat, NewLocation, FVector(1.0f, 1.0f, 1.0f));
	//ControlRigComponent->SetControlTransform(EffectorControlName, NewTransform, EControlRigComponentSpace::WorldSpace);
	//FVector ELoc = ControlRigComponent->GetControlTransform(TEXT("Robot1_End_ctrl"), EControlRigComponentSpace::WorldSpace).GetLocation();
	//UE_LOG(LogTemp, Warning, TEXT("GA_Rotate : ETrs X : %f, Y : %f, Z : %f"),
	//	ETrs.GetLocation().X, ETrs.GetLocation().Y, ETrs.GetLocation().Z);
	if (Alpha >= 1.0f)
	{
		OnFinishedRotate.Broadcast(NewRot);
		EndTask();
	}
}

void UDFAT_SoftRotate::OnDestroy(bool AbilityIsEnding)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityIsEnding);
}

