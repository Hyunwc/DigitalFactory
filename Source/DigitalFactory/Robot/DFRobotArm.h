// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DFRobotArm.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UControlRigComponent;
class UDFAbilitySystemComponent;

UCLASS()
class DIGITALFACTORY_API ADFRobotArm : public APawn
{
	GENERATED_BODY()

public:
	ADFRobotArm();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 로봇암의 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* Skeletal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UControlRigComponent* ControlRigComponent;

	// 이펙터가될 컨트롤릭 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FName EndEffectorName;

	// 회전중심이될 컨트롤릭 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FName RotatorName;

};
