// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DFRobotArm.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UControlRigComponent;
class UDFAbilitySystemComponent;
class UGameplayAbility;
class USphereComponent;
class UDFGA_RobotArmMasterAbility;

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

	// 감지용 콜리전
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SearchComp;

	// 이펙터가될 컨트롤릭 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FName EndEffectorName;

	// 회전중심이될 컨트롤릭 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FName RotatorName;

	// End의 복귀 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FTransform EndHomeTransform;

	// A의 복귀 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	FRotator AHomeRotate;

	// 현재 타겟 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlRig")
	AActor* TargetActor;

public:
	// ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UDFAbilitySystemComponent* DFASC;

	// Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> RobotArmMasterAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> FindAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> TargetAttachAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> ReturnToHomeAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> CombineTireAbility;

public:
	FGameplayAbilitySpecHandle MasterSpecHandle;
	FGameplayAbilitySpecHandle FindSpecHandle;
	FGameplayAbilitySpecHandle TargetAttachSpecHandle;
	FGameplayAbilitySpecHandle ReturnToHomeSpecHandle;
	FGameplayAbilitySpecHandle CombineSpecHandle;

	UPROPERTY()
	UDFGA_RobotArmMasterAbility* ActiveMaster;

public:
	UFUNCTION(BlueprintCallable)
	UDFAbilitySystemComponent* GetDFAbilitySystemComponent();

	// 트림셀이 호출하면 어빌리티 시작
	UFUNCTION(BlueprintCallable)
	void StartRobotArmAbility();

	UFUNCTION(BlueprintCallable)
	void NotifySubAbilityFinished();
};
