// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle/DFVehicleBase.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ADFVehicleBase::ADFVehicleBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Chassis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis"));
	Chassis->SetupAttachment(Root);
}


