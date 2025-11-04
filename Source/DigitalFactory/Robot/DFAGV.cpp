// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot/DFAGV.h"
#include "AI/DFAIController.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Vehicle/DFVehicleBase.h"
#include "Cell/DFCellBase.h"

ADFAGV::ADFAGV()
{
	//PrimaryActorTick.bCanEverTick = true;

	// AIController 설정
	AIControllerClass = ADFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -20.0f), FRotator(0.0f, 0.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(2.0f, 2.0f, 1.0f));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AGVMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/_GENERATED/wogns/SKM_AGV2.SKM_AGV2'"));
	if (AGVMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(AGVMeshRef.Object);
	}

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	// 이동 방향으로 회전
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	FindCellNum = 0;

	DFASC = CreateDefaultSubobject<UDFAbilitySystemComponent>(TEXT("DFASC"));

	AGVPhaseTag = FGameplayTag::RequestGameplayTag(TEXT("AGV.Phase.None"));
	AGVStateTag = FGameplayTag::RequestGameplayTag(TEXT("AGV.State.Idle"));
}

void ADFAGV::BeginPlay()
{
	Super::BeginPlay();
	
	//if (DFASC)
	//{
	//	DFASC->InitAbilityActorInfo(this, this);
	//
	//	FGameplayAbilitySpec AbilitySpec(Abilities);
	//	DFASC->GiveAbility(AbilitySpec);
	//}
}

void ADFAGV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADFAGV::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UDFAbilitySystemComponent* ADFAGV::GetDFAbilitySystemComponent() const
{
	return DFASC;
}

void ADFAGV::SetAGVPhaseTag(FGameplayTag NewPhaseTag)
{
	AGVPhaseTag = NewPhaseTag;

	OnChangeAGVPhase.Broadcast(AGVPhaseTag);
}

void ADFAGV::SetAGVStateTag(FGameplayTag NewStateTag)
{
	AGVStateTag = NewStateTag;

	OnChangeAGVState.Broadcast(AGVStateTag);
}

void ADFAGV::SetCurrentCell(ADFCellBase* NewCell)
{
	CurrentCell = NewCell;

	OnChangeAGVCurrentCell.Broadcast(CurrentCell);
}

void ADFAGV::SetOrderColor(FLinearColor NewOrderColor)
{
	OrderColor = NewOrderColor;

	OnChangeAGVOrderColor.Broadcast(OrderColor);
}

