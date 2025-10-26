// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "DFAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Cell/DFCellBase.h"
#include "Robot/DFAGV.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 만약 Idle이면
	ADFAGV* AGV = Cast<ADFAGV>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AGV)
	{
		return EBTNodeResult::Failed;
	}

	ADFCellBase* TargetCell = Cast<ADFCellBase>(BlackboardComp->GetValueAsObject(BBKEY_TARGETCELL));
	if (!TargetCell)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = TargetCell->GetActorLocation() - AGV->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	AGV->SetActorRotation(FMath::RInterpTo(AGV->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 500.0f));

	return EBTNodeResult::Succeeded;
}
