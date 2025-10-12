// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell/DFCellBase.h"
#include "DFInspectionCell.generated.h"

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFInspectionCell : public ADFCellBase
{
	GENERATED_BODY()
	
public:
	ADFInspectionCell();

protected:
	virtual void StartWork(ADFAGV* TargetAGV) override;
};
