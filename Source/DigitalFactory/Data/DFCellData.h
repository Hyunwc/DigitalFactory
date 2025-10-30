// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Cell/DFCellBase.h"
#include "GameplayTagContainer.h"
#include "DFCellData.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFCellData : public UObject
{
	GENERATED_BODY()

public:
	UDFCellData();

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	//TMap<FGameplayTag, FWrapperCellArray> CellMap;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	//TArray<ADFCellBase*> CellArray;
};
