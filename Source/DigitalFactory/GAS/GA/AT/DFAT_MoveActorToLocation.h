// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DFAT_MoveActorToLocation.generated.h"

class UCurveFloat;
class UCurveVector;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveActorToLocationDelegate);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFAT_MoveActorToLocation : public UAbilityTask
{
	GENERATED_BODY()

public:
	UDFAT_MoveActorToLocation(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(BlueprintAssignable)
	FMoveActorToLocationDelegate OnTargetLocationReached;

public:
	// 액터(ActorToMve)를 목표 위치로 Duration동안 부드럽게 이동시키는 함수
	// OwningAbility : 이 Task를 소유한 GA
	// ActorToMove : 실제로 이동시킬 액터(아바타가 아님)
	// Location : 목표 위치
	// Duration : 이동에 걸리는 시간
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UDFAT_MoveActorToLocation* MoveActorToLocation(
		UGameplayAbility* OwningAbility, 
		FName TaskInstanceName,
		AActor* ActorToMove,
		FVector Location, 
		float Duration, 
		UCurveFloat* OptionalInterpolationCurve,
		UCurveVector* OptionalVectorInterpolationCurve);

	// Ability Task 함수들 재정의
	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:
	// 이동시킬 대상 액터에 대한 참조
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	bool bIsFinished;

	UPROPERTY(Replicated)
	FVector StartLocation;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	float DurationOfMovement;

	float TimeMoveStarted;

	float TimeMoveWillEnd;

	// 보간 커브
	UPROPERTY(Replicated)
	TObjectPtr<UCurveFloat> LerpCurve;

	UPROPERTY(Replicated)
	TObjectPtr<UCurveVector> LerpCurveVector;
};
