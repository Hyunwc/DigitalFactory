// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFLoadCell.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Vehicle/DFVehicleBase.h"

ADFLoadCell::ADFLoadCell()
{
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);

	Distance = 0.0f;
	MoveSpeed = 100.0f;
}

void ADFLoadCell::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void ADFLoadCell::StartWork(ADFAGV* TargetAGV)
{
	Super::StartWork(TargetAGV);

	// 어빌리티 활성화
	if (DFASC && CellWorkAbilityClass)
	{
		FGameplayAbilitySpec* Spec = DFASC->FindAbilitySpecFromClass(CellWorkAbilityClass);
		FGameplayAbilitySpecHandle AbilityHandle;

		if (!Spec)
		{
			AbilityHandle = DFASC->GiveAbility(
				FGameplayAbilitySpec(CellWorkAbilityClass, 1, 0, this));
		}
		else
		{
			AbilityHandle = Spec->Handle;
		}

		if (AbilityHandle.IsValid())
		{
			DFASC->TryActivateAbility(AbilityHandle);
		}
	}
}

void ADFLoadCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Vehicle)
	{
		//const float SplineLength = Spline->GetSplineLength();

		// 거리 업데이트
		Distance += MoveSpeed * DeltaTime;
		FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		Vehicle->SetActorLocation(NewLocation);
		Vehicle->SetActorRotation(FRotator::ZeroRotator);

		if (Distance > Spline->GetSplineLength())
		{
			OnCompleteVehicleStatus.Broadcast(Vehicle->Color);
			OnVehicleLoadComplete.Broadcast();
		}
	}
}

void ADFLoadCell::InitCellData()
{
	Vehicle = nullptr;
	Distance = 0.0f;
	SetActorTickEnabled(false);
}
