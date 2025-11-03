// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ResetAGV.h"
#include "Robot/DFAGV.h"
#include "AIController.h"
#include "Manager/DFAGVManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ResetAGV::UBTTask_ResetAGV()
{
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
	NodeName = TEXT("ResetAGV");
}

EBTNodeResult::Type UBTTask_ResetAGV::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AGV)
	{
		return EBTNodeResult::Failed;
	}

	AGV->SetAGVPhaseTag(FGameplayTag::RequestGameplayTag("AGV.Phase.None"));
	AGV->SetAGVStateTag(FGameplayTag::RequestGameplayTag("AGV.State.Idle"));

	// 매니저의 GetIdleCount 호출시켜서 작업가능한 수량 갱신
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADFAGVManager::StaticClass(), FoundActors);
	if (ADFAGVManager* Manager = Cast<ADFAGVManager>(FoundActors[0]))
	{
		Manager->RenewalIdleAGVCount();
	}

	return EBTNodeResult::Succeeded;
}
