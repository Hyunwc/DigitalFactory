// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell/DFCellBase.h"
#include "DFLoadCell.generated.h"

class USplineComponent;
class ADFVehicleBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVehicleLoadComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteVehicleStatus, FLinearColor, Color);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFLoadCell : public ADFCellBase
{
	GENERATED_BODY()

public:
	ADFLoadCell();

protected:
	virtual void BeginPlay() override;
	virtual void StartWork(ADFAGV* TargetAGV) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* Spline;

	// 현재 AGV가 들고있는 차체
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFVehicleBase* Vehicle;

	UPROPERTY(VisibleAnywhere, Category = "Car")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	float MoveSpeed;

	// 현재 작업이 완료된 차 현황(Pair<색상, 수량>)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	//TMap<FLinearColor, int32> CompleteCarMap;

	UPROPERTY(BlueprintAssignable)
	FOnVehicleLoadComplete OnVehicleLoadComplete;

	UPROPERTY(BlueprintAssignable)
	FOnCompleteVehicleStatus OnCompleteVehicleStatus;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OpenDoor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CloseDoor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void VehicleLoadToSpline();

	UFUNCTION(BlueprintCallable)
	void InitCellData();

};
