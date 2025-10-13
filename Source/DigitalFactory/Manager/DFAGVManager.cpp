// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DFAGVManager.h"
#include "Robot/DFAGV.h"
#include "AI/DFAIController.h"

ADFAGVManager::ADFAGVManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADFAGVManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADFAGVManager::CommandWork()
{
	ADFAGV* Target = nullptr;

	for (ADFAGV* AGV : AGVList)
	{
		// 하나라도 찾으면 루프 탈출
		if (AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle")))
		{
			UE_LOG(LogTemp, Log, TEXT("AGVManager : 대기중인애 찾았다!"));
			Target = AGV;
			break;
		}
	}

	if (Target)
	{
		if (ADFAIController* AGVAICon = Cast<ADFAIController>(Target->GetController()))
		{
			AGVAICon->RunAI();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AGVManager : AICon이 유효하지가 않아"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AGVManager : Target이 유효하지가 않아"));
	}
}

