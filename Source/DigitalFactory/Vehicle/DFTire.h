// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFTire.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class DIGITALFACTORY_API ADFTire : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFTire();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	// 결합됐는지 안됐는지 확인하기 위한 불리언 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool bCombined;
};
