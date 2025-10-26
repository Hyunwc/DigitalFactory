// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFTrimCell.h"
#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Robot/DFRobotArm.h"

ADFTrimCell::ADFTrimCell()
{
	CellWorkAbilityClass = UDFGA_SimpleTimerWork::StaticClass();
	bRightRobotArmFinished = false;
	bLeftRobotArmFinished = false;
}

void ADFTrimCell::StartWork(ADFAGV* TargetAGV)
{
	// 비어 있어서 굳이 Super 호출 필요x. 나중에 CellBase에 로직추가될거 고려해서 일단 주석 처리
	//Super::StartWork(TargetAGV);

	if (LeftRobotArm && RightRobotArm)
	{
		UE_LOG(LogTemp, Log, TEXT("트림셀 : 구독할게요"));
		LeftRobotArm->OnRobotArmFinished.AddDynamic(this, &ADFTrimCell::OnFinishTrimCellWork);
	}

	LeftRobotArm->StartRobotArmAbility();
	RightRobotArm->StartRobotArmAbility();
	LeftRobotArm->TargetAGV = TargetAGV;
	RightRobotArm->TargetAGV = TargetAGV;
	
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
	if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.Type.Left")))
	{
		// 음.. 굳이 필요 없는 로직이였는데 일단 테스트
		bLeftRobotArmFinished = bFinished;
	}
	else if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("RobotArm.Type.Right")))
	{
		// 음.. 굳이 필요 없는 로직이였는데 일단 테스트
		bRightRobotArmFinished = bFinished;
	}

	UE_LOG(LogTemp, Warning, TEXT("트림셀 : 수고했어 로봇암아"));

	UE_LOG(LogTemp, Log, TEXT("트림셀 : 여기 들어오니?"));
	this->GetDFAbilitySystemComponent()->SetCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free"));

	OnCellWorkComplete.Broadcast(this);
	if (bLeftRobotArmFinished && bRightRobotArmFinished)
	{
	}
}
