// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot/DFAGV.h"
#include "AI/DFAIController.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ADFAGV::ADFAGV()
{
	//PrimaryActorTick.bCanEverTick = true;

	// AIController 설정
	AIControllerClass = ADFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	// 이동 로직 컴포넌트 설정
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 400.0f;
}

void ADFAGV::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADFAGV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADFAGV::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

