// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SmoothMove.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Robot/DFAGV.h"
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

	ADFAGV* OwnerAGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());

	CurrentLocation = OwnerAGV->GetActorLocation();

	// 만약 AGV가 복귀중이라면 
	if (OwnerAGV->AGVPhaseTag.MatchesTag(FGameplayTag::RequestGameplayTag("AGV.Phase.Home")))
	{
		TargetLocation = BlackboardComp->GetValueAsVector(BBKEY_HOMEPOS);
	}
	else
	{
		TargetLocation = BlackboardComp->GetValueAsVector(BBKEY_NEXTPOS);

	}

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
	NewLocation.Z = AIOwner->GetActorLocation().Z; 

	// 회전 보간
	//FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal2D();
	//if (!Direction.IsNearlyZero())
	//{
	//	float RawYaw = Direction.Rotation().Yaw;
	//	float SnappedYaw = FMath::RoundToFloat(RawYaw / 90.0f) * 90.0f;
	//
	//	FRotator TargetRotation = FRotator(0.0f, SnappedYaw, 0.0f);
	//	FRotator CurrentRotation = AIOwner->GetActorRotation();
	//
	//	FRotator NewRotation = FMath::RInterpTo(
	//		CurrentRotation,
	//		TargetRotation,
	//		DeltaSeconds,
	//		5.0f
	//	);
	//	
	//	AIOwner->SetActorRotation(NewRotation);
	//	UE_LOG(LogTemp, Warning, TEXT("BTSmooth : 새로운 로테이션 %f"), NewRotation.Yaw);
	//}
	AIOwner->SetActorLocation(NewLocation);

	if (Alpha >= 1.0f)
	{
		//FVector FinalDirection = (TargetLocation - CurrentLocation).GetSafeNormal2D();
		//if (!FinalDirection.IsNearlyZero())
		//{
		//	float RawYaw = FinalDirection.Rotation().Yaw;
		//	float SnappedYaw = FMath::RoundToFloat(RawYaw / 90.0f) * 90.0f;
		//
		//	UE_LOG(LogTemp, Warning, TEXT("BTSmooth : 최종 로테이션 %f"), FinalDirection.Rotation().Yaw);
		//	AIOwner->SetActorRotation(FRotator(0.0f, SnappedYaw, 0.0f));
		//}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
