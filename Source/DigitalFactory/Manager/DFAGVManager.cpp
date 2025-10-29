// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/DFAGVManager.h"
#include "Robot/DFAGV.h"
#include "AI/DFAIController.h"
#include "DFStruct.h"

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

void ADFAGVManager::StartOrder(FDFOrderSpec Spec)
{
	int32 AssignedCount = 0; 

	for (ADFAGV* AGV : AGVList)
	{
		// 필요한 주문 수량만큼 AGV를 찾았다면 루프 종료
		if (AssignedCount >= Spec.OrderQuantity)
		{
			break;
		}

		if (AGV && AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle")))
		{
			// AGV에게 색상 할당
			AGV->SetOrderColor(Spec.OrderColor);
			// 컨트롤러 구동
			if (ADFAIController* AGVAICon = Cast<ADFAIController>(AGV->GetController()))
			{
				AGVAICon->RunAI();
				AssignedCount++;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AGVManager : AICon이 유효하지가 않아"));
			}
		}
	}

	int32 NewCount = GetIdleAGVCount();
	// 현재 대기중인 차량의 수를 보냄
	OnIdleAGVCount.Broadcast(NewCount);
}

int32 ADFAGVManager::GetIdleAGVCount()
{
	int32 Count = 0;
	
	for (ADFAGV* AGV : AGVList)
	{
		if (AGV && AGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Idle")))
		{
			Count++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("AGVManager : 대기중인 AGV는 %d 대입니다"), Count);
	return Count;
}

