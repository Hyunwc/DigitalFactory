// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WaitCellWork.h"

UBTTask_WaitCellWork::UBTTask_WaitCellWork()
{
	bNotifyTick = true;
	WaitDuration = 3.0f; // 작업 시간
}

EBTNodeResult::Type UBTTask_WaitCellWork::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 작업 시작 시 경과시간 초기화
	ElapsedTime = 0.0f;
	return EBTNodeResult::InProgress;
}

void UBTTask_WaitCellWork::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= WaitDuration)
	{
		UE_LOG(LogTemp, Log, TEXT("작업 완료! 다음 셀로 가볼까?"));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
