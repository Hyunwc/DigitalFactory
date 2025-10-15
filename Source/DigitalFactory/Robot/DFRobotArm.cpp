// Fill out your copyright notice in the Description page of Project Settings.


#include "Robot/DFRobotArm.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ControlRigComponent.h"
#include "AI/RobotArm/DFRobotArmAIController.h"

ADFRobotArm::ADFRobotArm()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ADFRobotArmAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	Skeletal->SetupAttachment(Root);

	ControlRigComponent = CreateDefaultSubobject<UControlRigComponent>(TEXT("ControlRig"));
	ControlRigComponent->SetupAttachment(Root);

	EndEffectorName = "Robot1_End_ctrl";
	RotatorName = "Robot1_A_ctrl";


}

void ADFRobotArm::BeginPlay()
{
	Super::BeginPlay();
	
	// 컨트롤릭 스켈레탈에 결합
	ControlRigComponent->AddMappedCompleteSkeletalMesh(Skeletal);
}

void ADFRobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADFRobotArm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

