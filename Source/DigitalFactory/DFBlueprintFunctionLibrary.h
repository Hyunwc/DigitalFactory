// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DFBlueprintFunctionLibrary.generated.h"

class ADFAGV;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// UPARAM(ref) : 블루프린트에서 사용하기 위함
	// 액터전용 정렬함수
	UFUNCTION(BlueprintCallable)
	static void ActorArraySort(UPARAM(ref) TArray<AActor*>& ActorArray);

	// UPARAM(ref) : 블루프린트에서 사용하기 위함
	// AGV 전용
	UFUNCTION(BlueprintCallable)
	static void AGVArraySort(UPARAM(ref) TArray<ADFAGV*>& AGVArray);
};
