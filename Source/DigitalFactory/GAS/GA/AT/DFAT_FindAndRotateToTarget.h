// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DFAT_FindAndRotateToTarget.generated.h"

class UControlRigComponent;
class UCurveFloat;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFindAndRotateDelegate);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFAT_FindAndRotateToTarget : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UDFAT_FindAndRotateToTarget(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(BlueprintAssignable)
	FOnFindAndRotateDelegate OnFindAndRotateFinished;

public:
	/*로봇암의 컨트롤을 목표 타겟 액터의 소켓 방향으로 부드럽게 회전
	* ActorToRotate : GA의 아바타 액터(로봇암)
	* ControlName : 회전시킬 컨트롤릭의 컨트롤 이름
	* TargetSocketName : 탐지할 액터가 가져야할 소켓 이름
	*/ 
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", 
		meta = (DisplayName = "FindTargetAndRotate", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UDFAT_FindAndRotateToTarget* FindAndRotateToTargetTask(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		AActor* ActorToRotate,
		FName ControlName,
		FName TargetSocketName,
		float Duration = 1.0f
	);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:
	UPROPERTY()
	UControlRigComponent* ControlRigComponent;

	FName TargetControlName;
	FName TargetSocketNameToFind; // 탐지할 소켓 이름

	FVector TargetLocation; 

	FRotator StartRotation;
	FRotator TargetRotation;

	float DurationOfMovement = 0.0f;
	float TimeMoveStarted = 0.0f;
	float TimeMoveWillEnd = 0.0f;

	// 컨트롤릭 컴포넌트 찾는 함수
	bool InitializeControlRigComponent(AActor* TargetActor);

	bool FindTargetSocketLocation();
};
