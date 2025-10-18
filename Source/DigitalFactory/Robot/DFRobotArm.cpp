// Fill out your copyright notice in the Description page of Project Settings.


#include "Robot/DFRobotArm.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ControlRigComponent.h"
#include "AI/RobotArm/DFRobotArmAIController.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "ControlRig.h"
#include "RigVMHost.h"

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

	SearchComp = CreateDefaultSubobject<USphereComponent>(TEXT("SearchComp"));
	SearchComp->SetupAttachment(Root);

	EndEffectorName = "Robot1_End_ctrl";
	RotatorName = "Robot1_A_ctrl";

	DFASC = CreateDefaultSubobject<UDFAbilitySystemComponent>(TEXT("DFASC"));
}

void ADFRobotArm::BeginPlay()
{
	Super::BeginPlay();
	
	// 컨트롤릭 스켈레탈에 결합
	ControlRigComponent->AddMappedCompleteSkeletalMesh(Skeletal);

	URigVMHost* Host = ControlRigComponent->GetControlRig();

	Host->SetPublicVariableValue(TEXT("EndWeight"), 0.0f);
	Host->RequestInit();
}

void ADFRobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADFRobotArm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UDFAbilitySystemComponent* ADFRobotArm::GetDFAbilitySystemComponent()
{
	return DFASC;
}

void ADFRobotArm::StartRobotArmAbility()
{
	if (DFASC && RobotArmAbility)
	{
		FGameplayAbilitySpecHandle SpecHandle = DFASC->GiveAbility(
			FGameplayAbilitySpec(RobotArmAbility, 1, 0, this));

		if (SpecHandle.IsValid())
		{
			DFASC->TryActivateAbility(SpecHandle);
		}
	}
}

