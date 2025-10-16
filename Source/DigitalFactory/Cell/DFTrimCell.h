// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell/DFCellBase.h"
#include "DFTrimCell.generated.h"

class ADFRobotArm;
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
};
