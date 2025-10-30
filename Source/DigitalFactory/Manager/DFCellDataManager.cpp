// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DFCellDataManager.h"
#include "Cell/DFCellBase.h"
#include "Kismet/GameplayStatics.h"

void FWrapperCellArray::Sort()
{
	//TArray<ADFCellBase*> WrapperArray;
	WrapperArray.Sort([](const ADFCellBase& A, const ADFCellBase& B) 
		{
			return A.CellPriority < B.CellPriority;
		});
}

ADFCellDataManager::ADFCellDataManager()
{
	PrimaryActorTick.bCanEverTick = false;

	CellMap.Add(FGameplayTag::RequestGameplayTag("Cell.Type.Supply"));
	CellMap.Add(FGameplayTag::RequestGameplayTag("Cell.Type.Flexible"));
	CellMap.Add(FGameplayTag::RequestGameplayTag("Cell.Type.Trim"));
	CellMap.Add(FGameplayTag::RequestGameplayTag("Cell.Type.Inspection"));
	CellMap.Add(FGameplayTag::RequestGameplayTag("Cell.Type.Load"));
}

void ADFCellDataManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundCells;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADFCellBase::StaticClass(), FoundCells);

	for (AActor* Actor : FoundCells)
	{
		ADFCellBase* Cell = Cast<ADFCellBase>(Actor);

		if (Cell)
		{
			CellMap.FindOrAdd(Cell->CellTypeTag).WrapperArray.Add(Cell);
		}
	}

	for (auto& Pair : CellMap)
	{
		Pair.Value.Sort();
	}
}

TArray<ADFCellBase*> ADFCellDataManager::GetCellArray(FGameplayTag CellTypeTag)
{
	return CellMap.Find(CellTypeTag)->WrapperArray;
}

