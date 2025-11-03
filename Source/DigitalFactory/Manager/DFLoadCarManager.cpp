// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DFLoadCarManager.h"
#include "Cell/DFLoadCell.h"
#include "Kismet/GameplayStatics.h"

ADFLoadCarManager::ADFLoadCarManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADFLoadCarManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADFLoadCell::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ADFLoadCell* LoadCell = Cast<ADFLoadCell>(Actor))
		{
			LoadCell->OnCompleteVehicleStatus.AddDynamic(this, &ADFLoadCarManager::CacheCarStatus);
		}
	}
}

void ADFLoadCarManager::CacheCarStatus(FLinearColor Color)
{
	// Key에 해당하는 Color가 있으면 Value를 반환. 없으면 Key 추가 및 Value를 0으로
	int32& Count = CacheCompleteCarMap.FindOrAdd(Color, 0);
	Count++;

	OnChangeCarStatus.Broadcast(Color, Count);
}

