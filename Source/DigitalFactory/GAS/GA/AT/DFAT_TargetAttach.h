// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DFAT_TargetAttach.generated.h"

class UControlRigComponent;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetAttachDelegate);

UENUM(BlueprintType)
enum class EAttachPhase : uint8
{
	Initialized,
	MoveToTarget,
	Attaching,
	MoveToReturn,
	Finished
};
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFAT_TargetAttach : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UDFAT_TargetAttach(const FObjectInitializer& ObjectInitializer);

public:
	/* 이펙터를 타겟으로 향해 부드럽게 이동
	* Target : 어태치할 타겟 액터
	* ControlName : 회전시킬 컨트롤릭의 컨트롤 이름,
	* ControlComp : 이펙터의 정보를 빼내기 위한 컨트롤릭 컴포넌트
	* TargetLocation : 이펙터가 도달해야 할 타겟의 위치
	* ReturnTransform : 이펙터가 복귀해야 할 위치
	*/
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "TargetAttachAndMove", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UDFAT_TargetAttach* TargetAttachAndMove(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		AActor* Target,
		FName ControlName,
		UControlRigComponent* ControlComp,
		const FVector& TargetLocation,
		const FTransform& ReturnTransform,
		float Duration = 1.0f
	);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

public:
	// 작업이 끝냈다고 신호를 보낼 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnTargetAttachDelegate OnTargetAttachFinished;

	// 현재 Phase
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	EAttachPhase CurrentPhase;

protected:
	UPROPERTY()
	UControlRigComponent* ControlRigComponent;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	FName EffectorControlName;

	// 타겟 위치
	UPROPERTY()
	FVector TargetLocation;

	// 복귀 위치
	UPROPERTY()
	FTransform ReturnTransform;

	UPROPERTY()
	float DurationOfMovement;

	// 각 Phase의 시작 시간
	UPROPERTY()
	float TimePhaseStarted;

	//UPROPERTY()
	//FVector CurrentMoveStartLocation;

	//UPROPERTY()
	//FTransform CurrentMoveTransform;
};
