// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell/DFCellBase.h"
#include "DFSupplyCell.generated.h"

class USplineComponent;
class ADFVehicleBase;
class ADFAGV;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVehicleSplineEnd);

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFSupplyCell : public ADFCellBase
{
	GENERATED_BODY()
	
public:
	ADFSupplyCell();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void StartWork(ADFAGV* TargetAGV) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	USceneComponent* SpawnComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	FVector SplineStartLocation;

	// 스폰시킬 차체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Car")
	TSubclassOf<ADFVehicleBase> VehicleClass;

	// 스폰한 차체
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	ADFVehicleBase* SupplyVehicle;

	// 차체를 부착시킬 AGV 참조.
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Car")
	//ADFAGV* TargetAGV;

	UPROPERTY(VisibleAnywhere, Category = "Car")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
	float MoveSpeed;

	UPROPERTY(BlueprintAssignable)
	FOnVehicleSplineEnd OnVehicleSplineEnd;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnVehicle();

	UFUNCTION(BlueprintCallable)
	void InitCellData();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AttachVehicle(FName AttachSocketName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OpenDoor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CloseDoor();
};
