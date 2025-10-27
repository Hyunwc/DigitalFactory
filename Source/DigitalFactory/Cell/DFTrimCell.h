// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell/DFCellBase.h"
#include "GameplayTagContainer.h"
#include "DFTrimCell.generated.h"

class ADFRobotArm;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFTrimCell : public ADFCellBase
{
	GENERATED_BODY()
	
public:
	ADFTrimCell();

protected:
	virtual void StartWork(ADFAGV* TargetAGV) override;

public:
	// TODO : 컨테이너로 관리하는거 고려
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotArm")
	ADFRobotArm* LeftRobotArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RobotArm")
	ADFRobotArm* RightRobotArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RobotArm")
	bool bRightRobotArmFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RobotArm")
	bool bLeftRobotArmFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car")
	UStaticMeshComponent* LeftTireMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Car")
	UStaticMeshComponent* RightTireMesh;

public:
	UFUNCTION()
	void OnFinishTrimCellWork(FGameplayTag Tag, bool bFinished);

	// 블루프린트에서 구현해서 타이어 결합 어빌리티가 C++에서 호출함
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LeftTireSpawn();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RightTireSpawn();

	// 타이어 스폰을 준비해라고 알림받을 콜백
	UFUNCTION()
	void HandleTireSpawn(FGameplayTag Tag);
};
