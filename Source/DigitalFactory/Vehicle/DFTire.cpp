// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/DFTire.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ADFTire::ADFTire()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	bCombined = false;
}

void ADFTire::BeginPlay()
{
	Super::BeginPlay();
	
}



