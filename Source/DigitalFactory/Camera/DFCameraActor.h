// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "DFCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	ADFCameraActor();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FName CameraTag;
};
