// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "DFRobotArm.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UControlRigComponent;
class UDFAbilitySystemComponent;
class UGameplayAbility;
class USphereComponent;
class UDFGA_RobotArmMasterAbility;
class ADFAGV;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRobotArmFinishedDelegate, FGameplayTag, OwnerTag, bool, bFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyTireSpawn, FGameplayTag, OwnerTag);

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

	// 로봇암의 이펙터의 씬루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* EffectorScene;

	// 시각적 효과를 위한 타이어 메시(이펙터 씬에 부착되어야함)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* PreviewTire;

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

	// 현재 셀에서 넘겨받은 AGV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	ADFAGV* TargetAGV;

	// 자신이 어떤 페이즈인지(앞, 뒤)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GAS")
	FGameplayTag PhaseTag;

	// 현재 자신이 어떤 타입인지(왼쪽, 오른쪽)
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GAS")
	FGameplayTag TypeTag;

	UPROPERTY(BlueprintAssignable)
	FOnRobotArmFinishedDelegate OnRobotArmFinished;

	UPROPERTY(BlueprintAssignable)
	FOnReadyTireSpawn OnReadyTireSpawn;

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

	// 현재 트림셀 종료 조건을 만족하기 위해 로봇암마다 어빌리티 태스크에 적용할 시간의 딜레이를 주기 위함.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GAS")
	float OwnerDuration;

public:
	UFUNCTION(BlueprintCallable)
	UDFAbilitySystemComponent* GetDFAbilitySystemComponent();

	// 트림셀이 호출하면 어빌리티 시작
	UFUNCTION(BlueprintCallable)
	void StartRobotArmAbility();

	UFUNCTION(BlueprintCallable)
	void NotifySubAbilityFinished();

	UFUNCTION()
	void HandleAbilityFinished(FGameplayTag OwnerTag, bool bFinished);

	// 타이어를 어태치 했다는 것을 알림 받을 콜백
	UFUNCTION()
	void HandleAttachFinished();
};
