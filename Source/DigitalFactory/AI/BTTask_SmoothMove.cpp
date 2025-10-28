// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SmoothMove.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DFAI.h"

UBTTask_SmoothMove::UBTTask_SmoothMove()
{
	bNotifyTick = true;
	bCreateNodeInstance = true; // AI가 같은 메모리를 참조하지 않기 위해 개별적인 노드를 만들게 설정
}

EBTNodeResult::Type UBTTask_SmoothMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	TargetLocation = BlackboardComp->GetValueAsVector(BBKEY_NEXTPOS);
	
	Delta = 0.0f;
	Duration = 0.5f;

	return EBTNodeResult::InProgress;
}

void UBTTask_SmoothMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* AIOwner = OwnerComp.GetAIOwner()->GetPawn();

	if (!AIOwner)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 시간 업데이트
	Delta += DeltaSeconds;

	// 보간 계산
	float Alpha = FMath::Clamp(Delta / Duration, 0.0f, 1.0f);

	FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, Alpha);

	AIOwner->SetActorLocation(NewLocation);

	if (Alpha >= 1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
