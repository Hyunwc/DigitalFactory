// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DFStruct.generated.h"

// 주문 정보가 담길 구조체
USTRUCT(BlueprintType)
struct FDFOrderSpec
{
	GENERATED_BODY()

	// 주문할 차량의 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	FLinearColor OrderColor;

	// 주문 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	int32 OrderQuantity;
};


