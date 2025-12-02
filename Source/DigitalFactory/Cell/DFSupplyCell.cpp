// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell/DFSupplyCell.h"
#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "GAS/DFAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Vehicle/DFVehicleBase.h"
#include "Robot/DFAGV.h"

ADFSupplyCell::ADFSupplyCell()
{
	PrimaryActorTick.bCanEverTick = true;
	//CellWorkAbilityClass = UDFGA_SimpleTimerWork::StaticClass();
	SpawnComp = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnComp"));
	SpawnComp->SetupAttachment(Root);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);

	Distance = 0.0f;
	MoveSpeed = 100.0f;
}

void ADFSupplyCell::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	SplineStartLocation = Spline->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);

	InitCellData();
}

void ADFSupplyCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("SupplyCell : 차체 옮기기 시작할게!"));
	if (SupplyVehicle)
	{
		//const float SplineLength = Spline->GetSplineLength();

		// 거리 업데이트
		Distance += MoveSpeed * DeltaTime;
		FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		SupplyVehicle->SetActorLocation(NewLocation);
		SupplyVehicle->SetActorRotation(FRotator::ZeroRotator);

		// 끝 도달시
		if (Distance > Spline->GetSplineLength())
		{
			OnVehicleSplineEnd.Broadcast();
		}
	}
}

void ADFSupplyCell::StartWork(ADFAGV* TargetAGV)
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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failde to give or activate %s Ability to %s"),
				*CellWorkAbilityClass->GetName(), *GetName());
		}
	}
}

void ADFSupplyCell::SpawnVehicle()
{
	SupplyVehicle = GetWorld()->SpawnActor<ADFVehicleBase>(VehicleClass, SplineStartLocation, FRotator::ZeroRotator);

	// 이 함수로 들어왔다는 것은 차체를 옮기기 위함이기에 여기서 true로
	SetActorTickEnabled(true);
}

void ADFSupplyCell::InitCellData()
{
	SupplyVehicle = nullptr;
	Distance = 0.0f;
	SetActorTickEnabled(false);
}

//void ADFSupplyCell::AttachVehicle()
//{
//	SetActorTickEnabled(false);
//}
