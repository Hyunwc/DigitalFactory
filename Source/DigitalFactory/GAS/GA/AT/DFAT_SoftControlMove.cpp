// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_SoftControlMove.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "ControlRigComponent.h"

UDFAT_SoftControlMove::UDFAT_SoftControlMove(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UDFAT_SoftControlMove* UDFAT_SoftControlMove::StartSoftMove(UGameplayAbility* OwningAbility, FName TaskInstanceName, UControlRigComponent* RigComp, FName ControlName, FVector Target, AActor* TargetActor, float Duration)
{
	// 객체 생성
	UDFAT_SoftControlMove* MyObj = NewAbilityTask<UDFAT_SoftControlMove>(OwningAbility, TaskInstanceName);

	if (MyObj)
	{
		MyObj->ControlRigComponent = RigComp;
		MyObj->TargetControlName = ControlName;
		MyObj->TargetLocation = Target;
		MyObj->TargetActor = TargetActor;
		MyObj->Duration = FMath::Max(Duration, 0.001f);
	}

	return MyObj;
}

void UDFAT_SoftControlMove::Activate()
{
	if (!ControlRigComponent)
	{
		EndTask();
		return;
	}

	StartTransform = ControlRigComponent->GetControlTransform(TargetControlName, EControlRigComponentSpace::WorldSpace);
	UE_LOG(LogTemp, Warning, TEXT("소프트무브 : StartTransform X : %f, Y : %f, Z : %f"),
		StartTransform.GetLocation().X, StartTransform.GetLocation().Y, StartTransform.GetLocation().Z);
	UE_LOG(LogTemp, Warning, TEXT("소프트무브 : TargetLoc X : %f, Y : %f, Z : %f"),
		TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
	ElapsedTime = 0.0f;
	bTickingTask = true;
}

void UDFAT_SoftControlMove::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	FTransform CurTrans = ControlRigComponent->GetControlTransform(TargetControlName, EControlRigComponentSpace::WorldSpace);
	FVector NewLoc = FMath::Lerp(StartTransform.GetLocation(), TargetLocation, Alpha);
	//FQuat NewQuat = FQuat::Slerp(StartTransform.GetRotation(), TargetActor->GetActorQuat(), Alpha).GetNormalized();
	//FQuat NewQuat = FQuat::Slerp(StartTransform.GetRotation(), TargetActor->GetActorQuat(), Alpha).GetNormalized();
	//FTransform NewTransform(NewQuat, NewLoc, FVector(1.0f, 1.0f, 1.0f));
	FTransform NewTransform(FRotator::ZeroRotator, NewLoc, FVector(1.0f, 1.0f, 1.0f));

	ControlRigComponent->SetControlTransform(TargetControlName, NewTransform, EControlRigComponentSpace::WorldSpace);

	if (Alpha >= 1.0f)
	{
		OnFinishedMove.Broadcast(NewTransform);
		EndTask();
	}
}

void UDFAT_SoftControlMove::OnDestroy(bool AbilityIsEnding)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityIsEnding);
}
