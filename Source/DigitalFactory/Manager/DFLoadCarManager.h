// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFLoadCarManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeCarStatus, FLinearColor, Color, int32, NewCount);

UCLASS()
class DIGITALFACTORY_API ADFLoadCarManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFLoadCarManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	TMap<FLinearColor, int32> CacheCompleteCarMap;

	UPROPERTY(BlueprintAssignable)
	FOnChangeCarStatus OnChangeCarStatus;

public:
	UFUNCTION(BlueprintCallable)
	void CacheCarStatus(FLinearColor Color);
};
