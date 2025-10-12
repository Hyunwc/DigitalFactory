// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFInspectionCell.h"
#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "GAS/DFAbilitySystemComponent.h"

ADFInspectionCell::ADFInspectionCell()
{
	CellWorkAbilityClass = UDFGA_SimpleTimerWork::StaticClass();
}

void ADFInspectionCell::StartWork(ADFAGV* TargetAGV)
{
	Super::StartWork(TargetAGV);

	// 어빌리티 활성화
	if (DFASC && CellWorkAbilityClass)
	{
		FGameplayAbilitySpecHandle AbilityHandle = DFASC->GiveAbility(
			FGameplayAbilitySpec(CellWorkAbilityClass, 1, 0, this)
		);

		if (AbilityHandle.IsValid())
		{
			DFASC->TryActivateAbility(AbilityHandle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failde to give or activate %s Ability to %s"),
				*CellWorkAbilityClass->GetName(), *GetName());
		}
	}
}
