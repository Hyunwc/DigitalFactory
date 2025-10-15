// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_MoveActorToLocation.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetMathLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DFAT_MoveActorToLocation)

UDFAT_MoveActorToLocation::UDFAT_MoveActorToLocation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	bSimulatedTask = true;
	bIsFinished = false;

	DurationOfMovement = 0.0f;
	TimeMoveStarted = 0.0f;
	TimeMoveWillEnd = 0.0f;
}

UDFAT_MoveActorToLocation* UDFAT_MoveActorToLocation::MoveActorToLocation(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* ActorToMove, FVector Location, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve)
{
	// 객체 생성
	UDFAT_MoveActorToLocation* MyObj = NewAbilityTask< UDFAT_MoveActorToLocation>(OwningAbility, TaskInstanceName);

	// 이동 대상 액터 저장
	MyObj->TargetActor = ActorToMove;

	// 시작 위치 설정
	if (MyObj->TargetActor != nullptr)
	{
		MyObj->StartLocation = MyObj->TargetActor->GetActorLocation();
	}
	else
	{
		MyObj->bTickingTask = false;
	}

	MyObj->TargetLocation = Location;
	MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);
	MyObj->LerpCurve = OptionalInterpolationCurve;
	MyObj->LerpCurveVector = OptionalVectorInterpolationCurve;

	return MyObj;
}

void UDFAT_MoveActorToLocation::Activate()
{
	TimeMoveStarted = GetWorld()->GetTimeSeconds();
	TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;

	if (TargetActor == nullptr)
	{
		OnTargetLocationReached.Broadcast();
		EndTask();
		return;
	}

	// 혹시 모르니 다시 명시
	bTickingTask = true;
}

void UDFAT_MoveActorToLocation::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (TargetActor == nullptr)
	{
		OnTargetLocationReached.Broadcast();
		EndTask();
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - TimeMoveStarted;

	// 보간 비율 (0.0에서 1.0)
	float LerpAlpha = FMath::Clamp(TimeElapsed / DurationOfMovement, 0.0f, 1.0f);

	if (LerpAlpha >= 1.0f)
	{
		// 1.0f 도달 : 최종 위치로 설정
		TargetActor->SetActorLocation(TargetLocation);
		OnTargetLocationReached.Broadcast();
		EndTask();
	}
	else
	{
		FVector NewLocation;

		// 커브 또는 선형 보간 적용
		if (LerpCurveVector)
		{
			// Vector Curve 사용(X, Y, Z 축별 보간)
			NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpCurveVector->GetVectorValue(LerpAlpha));
		}
		else if (LerpCurve)
		{
			// Float Curve 사용 (단일 스칼라 보간)
			NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpCurve->GetFloatValue(LerpAlpha));
		}
		else
		{
			// 기본 선형 보간
			NewLocation = FMath::Lerp(StartLocation, TargetLocation, LerpAlpha);
		}

		// 액터 위치 업데이트

		TargetActor->SetActorLocation(NewLocation);
	}
}

void UDFAT_MoveActorToLocation::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UDFAT_MoveActorToLocation, StartLocation);
	//DOREPLIFETIME(UDFAT_MoveActorToLocation, TargetLocation);
	//DOREPLIFETIME(UDFAT_MoveActorToLocation, DurationOfMovement);
	//DOREPLIFETIME(UDFAT_MoveActorToLocation, LerpCurve);
	//DOREPLIFETIME(UDFAT_MoveActorToLocation, LerpCurveVector);
}

void UDFAT_MoveActorToLocation::OnDestroy(bool AbilityIsEnding)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityIsEnding);
}
