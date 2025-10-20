// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFTrimCell.h"
#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Robot/DFRobotArm.h"
#include "GAS/GA/DFGA_RobotArmMasterAbility.h"

ADFTrimCell::ADFTrimCell()
{
	CellWorkAbilityClass = UDFGA_SimpleTimerWork::StaticClass();
}

void ADFTrimCell::StartWork(ADFAGV* TargetAGV)
{
	// 비어 있어서 굳이 Super 호출 필요x. 나중에 CellBase에 로직추가될거 고려해서 일단 주석 처리
	//Super::StartWork(TargetAGV);

	LeftRobotArm->StartRobotArmAbility();
	RightRobotArm->StartRobotArmAbility();
	LeftRobotArm->TargetAGV = TargetAGV;
	RightRobotArm->TargetAGV = TargetAGV;

	//LeftRobotArm->RobotArmMasterAbility.AddDynamic(this, &ADFTrimCell::OnFinishTrimCellWork);

	UDFGA_RobotArmMasterAbility* LeftRAGA = Cast<UDFGA_RobotArmMasterAbility>(LeftRobotArm->RobotArmMasterAbility);
	UDFGA_RobotArmMasterAbility* RightRAGA = Cast<UDFGA_RobotArmMasterAbility>(RightRobotArm->RobotArmMasterAbility);

	LeftRAGA->OnFinishTireAssembly.AddDynamic(this, &ADFTrimCell::OnFinishTrimCellWork);
	RightRAGA->OnFinishTireAssembly.AddDynamic(this, &ADFTrimCell::OnFinishTrimCellWork);
	// 어빌리티 활성화
	//if (DFASC && CellWorkAbilityClass)
	//{
	//	FGameplayAbilitySpecHandle AbilityHandle = DFASC->GiveAbility(
	//		FGameplayAbilitySpec(CellWorkAbilityClass, 1, 0, this)
	//	);
	//
	//	if (AbilityHandle.IsValid())
	//	{
	//		DFASC->TryActivateAbility(AbilityHandle);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Failde to give or activate %s Ability to %s"),
	//			*CellWorkAbilityClass->GetName(), *GetName());
	//	}
	//}
}

void ADFTrimCell::OnFinishTrimCellWork(FGameplayTag Tag, bool bFinished)
{
}
