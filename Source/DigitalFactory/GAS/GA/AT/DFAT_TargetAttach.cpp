// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_TargetAttach.h"
#include "GameFramework/Actor.h"
#include "ControlRigComponent.h"
#include "Kismet/KismetMathLibrary.h"

UDFAT_TargetAttach::UDFAT_TargetAttach(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DurationOfMovement = 1.0f;
	TimePhaseStarted = 0.0f;
	CurrentPhase = EAttachPhase::Initialized;
}


UDFAT_TargetAttach* UDFAT_TargetAttach::TargetAttachAndMove(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* Target, FName ControlName, UControlRigComponent* ControlComp, const FVector& TargetLocation, const FTransform& ReturnTransform, float Duration)
{
	UDFAT_TargetAttach* MyObj = NewAbilityTask<UDFAT_TargetAttach>(OwningAbility, TaskInstanceName);

	if (MyObj)
	{
		MyObj->TargetActor = Target;
		MyObj->EffectorControlName = ControlName;
		MyObj->ControlRigComponent = ControlComp;
		MyObj->TargetLocation = TargetLocation;
		MyObj->ReturnTransform = ReturnTransform;
		MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);

	}

	return MyObj;
}

void UDFAT_TargetAttach::Activate()
{
	if (ControlRigComponent == nullptr || TargetActor == nullptr)
	{
		OnTargetAttachFinished.Broadcast();
		EndTask();
		return;
	}
	ControlRigComponent->SetControlTransform(EffectorControlName, ReturnTransform, EControlRigComponentSpace::WorldSpace);
	// 복귀장소 캐시
	//ReturnTransform = ControlRigComponent->GetControlTransform(EffectorControlName, EControlRigComponentSpace::WorldSpace);
	// 이미 받았지만 혹시 모르니 다시 저장
	//FTransform StartTransform = ControlRigComponent->GetControlTransform(EffectorControlName, EControlRigComponentSpace::WorldSpace);
	//CurrentMoveStartLocation = StartTransform.GetLocation();

	UE_LOG(LogTemp, Warning, TEXT("AT_Target : ReturnTransform X : %f, Y : %f, Z : %f"),
		ReturnTransform.GetLocation().X, ReturnTransform.GetLocation().Y, ReturnTransform.GetLocation().Z);

	UE_LOG(LogTemp, Warning, TEXT("AT_Target : TargetActorLocation X : %f, Y : %f, Z : %f"),
		TargetLocation.X, TargetLocation.Y, TargetLocation.Z);

	CurrentPhase = EAttachPhase::MoveToTarget;
	TimePhaseStarted = GetWorld()->GetTimeSeconds();
	bTickingTask = true;
}

void UDFAT_TargetAttach::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ControlRigComponent == nullptr || CurrentPhase == EAttachPhase::Finished)
	{
		OnTargetAttachFinished.Broadcast();
		EndTask();
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - TimePhaseStarted;
	float LerpAlpha = FMath::Clamp(TimeElapsed / DurationOfMovement, 0.0f, 1.0f);

	FTransform CurrentEffectorTransform = ControlRigComponent->GetControlTransform(EffectorControlName, EControlRigComponentSpace::WorldSpace);

	switch (CurrentPhase)
	{
	case EAttachPhase::MoveToTarget:
	{
		// 타겟 위치로 이동

		//FVector NewLocation = FMath::Lerp(CurrentEffectorTransform.GetLocation(), TargetLocation, LerpAlpha);
		//FRotator CurrentRotate = CurrentEffectorTransform.GetRotation().Rotator();
		//FRotator TargetRotate = UKismetMathLibrary::FindLookAtRotation(CurrentEffectorTransform.GetLocation(), TargetLocation);
		//
		//// 회전 보간
		//FQuat NewQuat = FQuat::Slerp(CurrentRotate.Quaternion(), TargetRotate.Quaternion(), LerpAlpha).GetNormalized();
	
		FVector NewLocation = FMath::Lerp(CurrentEffectorTransform.GetLocation(), TargetLocation, LerpAlpha);
		//UE_LOG(LogTemp, Warning, TEXT("AT_Target : NewLocation X : %f, Y : %f, Z : %f"),
		//	NewLocation.X, NewLocation.Y, NewLocation.Z);
		FQuat NewQuat = FQuat::Slerp(CurrentEffectorTransform.GetRotation(), TargetActor->GetActorQuat(), LerpAlpha).GetNormalized();
	
		FTransform NewTransform(NewQuat, NewLocation, FVector(1.0f, 1.0f, 1.0f));
		ControlRigComponent->SetControlTransform(EffectorControlName, NewTransform, EControlRigComponentSpace::WorldSpace);

		//UE_LOG(LogTemp, Warning, TEXT("AT_Target : NewLocation X : %f, Y : %f, Z : %f"),
		//		NewLocation.X, NewLocation.Y, NewLocation.Z);
		if (LerpAlpha >= 1.0f)
		{
			// 페이즈 변경 (어태치로)
			CurrentPhase = EAttachPhase::Attaching;
			TimePhaseStarted = CurrentTime;
			//UE_LOG(LogTemp, Warning, TEXT("AT_Target : NewLocation X : %f, Y : %f, Z : %f"),
			//	NewLocation.X, NewLocation.Y, NewLocation.Z);
			// 복귀 이동 시작점을 현재 위치로 갱신
			//CurrentMoveStartTransform = CurrentEffectorTransform;
		}


		break;
	}
	case EAttachPhase::Attaching:
	{
		//FTransform CurrLoc = ControlRigComponent->GetControlTransform(EffectorControlName, EControlRigComponentSpace::WorldSpace);
		//UE_LOG(LogTemp, Warning, TEXT("AT_Target : 현재 이펙터의 위치 X : %f, Y : %f, Z : %f"),
		//	CurrLoc.GetLocation().X, CurrLoc.GetLocation().Y, CurrLoc.GetLocation().Z);
		// 어태치 로직 실행
		//bTickingTask = false;
		OnTargetAttachFinished.Broadcast();
		break;
	}
	}
}

void UDFAT_TargetAttach::OnDestroy(bool AbilityIsEnding)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityIsEnding);
}
