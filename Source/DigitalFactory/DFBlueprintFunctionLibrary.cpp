// Fill out your copyright notice in the Description page of Project Settings.


#include "DFBlueprintFunctionLibrary.h"
#include "Robot/DFAGV.h"

void UDFBlueprintFunctionLibrary::ActorArraySort(TArray<AActor*>& ActorArray)
{
	ActorArray.Sort([](const AActor& A, const AActor& B)
		{
			return A.GetName() < B.GetName();
		});
}

void UDFBlueprintFunctionLibrary::AGVArraySort(UPARAM(ref)TArray<ADFAGV*>& AGVArray)
{
	AGVArray.Sort([](const ADFAGV& A, const ADFAGV& B)
		{
			return A.AGVListNumber < B.AGVListNumber;
		});
}
