// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetAGVPhase.h"
#include "DFAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Robot/DFAGV.h"
#include "AIController.h"

UBTTask_SetAGVPhase::UBTTask_SetAGVPhase()
{
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
	NodeName = TEXT("Set AGV Phase");
	AGVPhaseKeyName = BBKEY_CURRENTPHASE;
}

EBTNodeResult::Type UBTTask_SetAGVPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드의 AGV Phase 키 값을 새로운 Phase 태그 이름으로 업데이트
	//BlackboardComp->SetValueAsName(AGVPhaseKeyName, NewAGVPhaseTagName);

	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	AGV->SetAGVPhaseTag(FGameplayTag::RequestGameplayTag(NewAGVPhaseTagName));

	return EBTNodeResult::Succeeded;
}
