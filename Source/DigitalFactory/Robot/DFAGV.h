// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameplayTagContainer.h"
#include "DFAGV.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;
class UDFAbilitySystemComponent;
class UGameplayAbility;
class ADFVehicleBase;

UCLASS()
class DIGITALFACTORY_API ADFAGV : public APawn
{
	GENERATED_BODY()

public:
	ADFAGV();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UDFAbilitySystemComponent* GetDFAbilitySystemComponent() const;

public:
	// AGV의 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	// AGV의 시각적 표현
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	// 이동 로직
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* Movement;

	// DF ASC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UDFAbilitySystemComponent* DFASC;

	// 어빌리티 클래스
	// TODO : 어빌리티가 늘어날 경우 컨테이너를 통해 관리하는 것으로 수정
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	//TSubclassOf<UGameplayAbility> Abilities;
	//
	// AGV의 Phase를 나타내는 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Phase")
	FGameplayTag AGVPhaseTag;

	// 현재 장착중인 차체 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	ADFVehicleBase* AttchVehicle;
};
