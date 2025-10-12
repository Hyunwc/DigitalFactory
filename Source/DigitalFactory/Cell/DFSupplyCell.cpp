// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFSupplyCell.h"
#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "AbilitySystemComponent.h"

ADFSupplyCell::ADFSupplyCell()
{
	CellWorkAbilityClass = UDFGA_SimpleTimerWork::StaticClass();
}

void ADFSupplyCell::StartWork(ADFAGV* TargetAGV)
{
	Super::StartWork(TargetAGV);

	// 어빌리티 활성화
	if (ASC && CellWorkAbilityClass)
	{
		FGameplayAbilitySpecHandle AbilityHandle = ASC->GiveAbility(
			FGameplayAbilitySpec(CellWorkAbilityClass, 1, 0, this)
		);

		if (AbilityHandle.IsValid())
		{
			ASC->TryActivateAbility(AbilityHandle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failde to give or activate %s Ability to %s"),
				*CellWorkAbilityClass->GetName(), *GetName());
		}
	}
}
