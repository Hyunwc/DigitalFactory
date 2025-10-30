// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFCellBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Robot/DFAGV.h"
#include "GAS/DFAbilitySystemComponent.h"

ADFCellBase::ADFCellBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	AGVTargetPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
	AGVTargetPoint->SetupAttachment(Root);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	DFASC = CreateDefaultSubobject<UDFAbilitySystemComponent>(TEXT("DFS"));

	CellStateTag = FGameplayTag::RequestGameplayTag("Cell.State.Free");
}

void ADFCellBase::BeginPlay()
{
	Super::BeginPlay();

	//if (ASC)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Cell : ASC 잘 부착되었습니다"));
	//}

	if (DFASC)
	{
		DFASC->InitAbilityActorInfo(this, this);

		DFASC->AddInitialCellStateTags(InitialCellStateTags);
	}
}

UAbilitySystemComponent* ADFCellBase::GetAbilitySystemComponent() const
{
	//return ASC;
	return DFASC;
}

UDFAbilitySystemComponent* ADFCellBase::GetDFAbilitySystemComponent() const
{
	return DFASC;
}

void ADFCellBase::StartWork(ADFAGV* TargetAGV)
{
}

void ADFCellBase::SetCellStateTag(FGameplayTag NewTag)
{
	CellStateTag = NewTag;

	OnChangeCellStateTag.Broadcast(CellStateTag);
}

void ADFCellBase::SetCellReservedAGV(ADFAGV* NewAGV)
{
	ReservedAGV = NewAGV;

	OnChangeReservedAGV.Broadcast(ReservedAGV);
}

void ADFCellBase::SetCellWorkingAGV(ADFAGV* NewAGV)
{
	WorkingAGV = NewAGV;

	OnChangeWorkingAGV.Broadcast(WorkingAGV);
}


