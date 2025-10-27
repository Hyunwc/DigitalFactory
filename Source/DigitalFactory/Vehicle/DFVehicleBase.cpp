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

	LeftFrontTire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftFrontTire"));
	LeftFrontTire->SetupAttachment(Chassis);

	LeftBackTire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBackTire"));
	LeftBackTire->SetupAttachment(Chassis);

	RightFrontTire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightFrontTire"));
	RightFrontTire->SetupAttachment(Chassis);

	RightBackTire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBackTire"));
	RightBackTire->SetupAttachment(Chassis);

	TireVisibleMap.Add(FGameplayTag::RequestGameplayTag("Vehicle.Tire.Front.Left"), LeftFrontTire);
	TireVisibleMap.Add(FGameplayTag::RequestGameplayTag("Vehicle.Tire.Back.Left"), LeftBackTire);
	TireVisibleMap.Add(FGameplayTag::RequestGameplayTag("Vehicle.Tire.Front.Right"), RightFrontTire);
	TireVisibleMap.Add(FGameplayTag::RequestGameplayTag("Vehicle.Tire.Back.Right"), RightBackTire);
}


