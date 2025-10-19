// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DFAT_SoftControlMove.generated.h"

class UControlRigComponent;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedMoveDelegate);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFAT_SoftControlMove : public UAbilityTask
{
	GENERATED_BODY()

public:
	UDFAT_SoftControlMove(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(BlueprintAssignable)
	FOnFinishedMoveDelegate OnFinishedMove;

public:
	/*로봇암의 컨트롤을 목표 타겟 액터의 소켓 방향으로 부드럽게 회전
	* ActorToRotate : GA의 아바타 액터(로봇암)
	* ControlName : 회전시킬 컨트롤릭의 컨트롤 이름
	* TargetSocketName : 탐지할 액터가 가져야할 소켓 이름
	*/
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "StartSoftMove", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UDFAT_SoftControlMove* StartSoftMove(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UControlRigComponent* RigComp,
		FName ControlName,
		FVector Target,
		AActor* TargetActor,
		float Duration
	);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:
	UPROPERTY()
	UControlRigComponent* ControlRigComponent;

	FName TargetControlName;

	FVector TargetLocation;

	FTransform StartTransform;

	AActor* TargetActor;

	float Duration = 0.0f;

	float ElapsedTime = 0.0f;
	float TimeMoveStarted = 0.0f;
	float TimeMoveWillEnd = 0.0f;
	
};
