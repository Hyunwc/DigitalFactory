// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DFVehicleBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class DIGITALFACTORY_API ADFVehicleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFVehicleBase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* Root;

	// 차체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* Chassis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* LeftFrontTire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* LeftBackTire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* RightFrontTire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* RightBackTire;

	// 바퀴가 전부 결합됐는지 체크할 컨테이너<Key : 태그(Vehicle.Type.앞or뒤.왼or오), Value : 결합됐는지?>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicle")
	TMap<FGameplayTag, bool> TireCombineMap;

public:
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetChassisMesh() { return Chassis; }
};
