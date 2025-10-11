// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFCellBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Robot/DFAGV.h"

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
}

void ADFCellBase::BeginPlay()
{
	Super::BeginPlay();

	if (ASC)
	{
		UE_LOG(LogTemp, Log, TEXT("Cell : ASC 잘 부착되었습니다"));
	}
}

UAbilitySystemComponent* ADFCellBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void ADFCellBase::StartWork(ADFAGV* TargetAGV)
{
}


