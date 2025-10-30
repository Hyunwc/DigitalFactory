// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DFCellDataManager.generated.h"

class ADFCellBase;

USTRUCT(BlueprintType)
struct FWrapperCellArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ADFCellBase*> WrapperArray;

	void Sort();
};

UCLASS()
class DIGITALFACTORY_API ADFCellDataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFCellDataManager();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	TMap<FGameplayTag, FWrapperCellArray> CellMap;

public:
	UFUNCTION(BlueprintCallable)
	TArray<ADFCellBase*> GetCellArray(FGameplayTag CellTypeTag);

};
