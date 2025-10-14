// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFVehicleBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class DIGITALFACTORY_API ADFVehicleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFVehicleBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* Root;

	// 차체
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* Chassis;

};
