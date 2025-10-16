// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_FindAndRotateToTarget.h"
#include "Robot/DFRobotArm.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Components/SphereComponent.h"
#include "ControlRigComponent.h"
#include "Components/MeshComponent.h"

UDFAT_FindAndRotateToTarget::UDFAT_FindAndRotateToTarget(const FObjectInitializer& ObjectInitializer)
{
	
}


UDFAT_FindAndRotateToTarget* UDFAT_FindAndRotateToTarget::FindAndRotateToTargetTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* ActorToRotate, FName ControlName, FName TargetSocketName, float Duration)
{
	// 객체 생성
	UDFAT_FindAndRotateToTarget* MyObj = NewAbilityTask<UDFAT_FindAndRotateToTarget>(OwningAbility, TaskInstanceName);

	if (MyObj->InitializeControlRigComponent(ActorToRotate))
	{
		MyObj->TargetControlName = ControlName;
		MyObj->TargetSocketNameToFind = TargetSocketName;
		MyObj->DurationOfMovement = FMath::Max(Duration, 0.001f);
	}
	else
	{
		MyObj->DurationOfMovement = 0.0f;
	}

	return MyObj;
}

void UDFAT_FindAndRotateToTarget::Activate()
{
	// 컨트롤릭이 없다면 즉시 종료
	if (ControlRigComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AT_FIind : 컨트롤릭을 못찾았어요"));
		EndTask();
		return;
	}

	if (!FindTargetSocketLocation())
	{
		UE_LOG(LogTemp, Error, TEXT("AT_FIind : 타겟 못찾았어요"));
		OnFindAndRotateFinished.Broadcast();
		EndTask();
		return;
	}

	// 현재 회전 및 목표 회전 계산
	FTransform ControlTransform = ControlRigComponent->GetControlTransform(TargetControlName, EControlRigComponentSpace::WorldSpace);

	StartRotation = ControlTransform.GetRotation().Rotator();

	// TargetControlName의 월드 위치를 기준으로 타겟을 바라보는 회전을 계산
	FVector StartLocation = ControlTransform.GetLocation();
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	TargetRotation = FRotator(StartRotation.Pitch, LookAtRotator.Yaw, StartRotation.Roll);

	// 시간 초기화 및 Tick시작
	TimeMoveStarted = GetWorld()->GetTimeSeconds();
	TimeMoveWillEnd = TimeMoveStarted + DurationOfMovement;
	bTickingTask = true;
}

void UDFAT_FindAndRotateToTarget::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ControlRigComponent == nullptr)
	{
		OnFindAndRotateFinished.Broadcast();
		EndTask();
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float TimeElapsed = CurrentTime - TimeMoveStarted;
	float LerpAlpha = FMath::Clamp(TimeElapsed / DurationOfMovement, 0.0f, 1.0f);

	FRotator NewRotation;
	UE_LOG(LogTemp, Log, TEXT("AT_FIind : 타겟이 유효하니 여기 들어왔지요~"));
	if (LerpAlpha >= 1.0f)
	{
		// 1.0f 도달 : 최종 회전 설정 및 Task 종료
		NewRotation = TargetRotation;

		OnFindAndRotateFinished.Broadcast();
		EndTask();
	}
	else
	{
		NewRotation = FMath::RInterpTo(StartRotation, TargetRotation, LerpAlpha, 1.0f);
	}

	// 컨트롤릭의 회전 업데이트
	ControlRigComponent->SetControlRotator(TargetControlName, NewRotation, EControlRigComponentSpace::WorldSpace);
}

void UDFAT_FindAndRotateToTarget::OnDestroy(bool AbilityIsEnding)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityIsEnding);
}

bool UDFAT_FindAndRotateToTarget::InitializeControlRigComponent(AActor* TargetActor)
{
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("AT_FIind : 타겟이 유효하지 않아요"));
		return false;
	}

	// 로봇암 액터 내에서 컨트롤릭을 찾는다.
	ControlRigComponent = TargetActor->GetComponentByClass<UControlRigComponent>();

	return ControlRigComponent != nullptr;
}

bool UDFAT_FindAndRotateToTarget::FindTargetSocketLocation()
{
	AActor* OwnerActor = GetAvatarActor(); // GA를 소유한 로봇암
	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("AT_FIind : 오너가 유효하지 않아요"));
		return false;
	}

	USphereComponent* SearchSphere = OwnerActor->FindComponentByClass<USphereComponent>();
	if (!SearchSphere)
	{
		UE_LOG(LogTemp, Error, TEXT("AT_FIind : 스피어가 유효하지 않아요"));
		return false;
	}

	TArray<AActor*> OverlappingActors;
	SearchSphere->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("AT_FIind : 1개도 못찾았어요"));
		return false;
	}

	// 순회해서 TargetSocket을 가진 액터 찾는다.
	for (AActor* TargetActor : OverlappingActors)
	{
		UE_LOG(LogTemp, Log, TEXT("AT_FIind : 오버랩에 들어있는 너의 이름은? %s"), *TargetActor->GetName());
		if (TargetActor)
		{
			UMeshComponent* Mesh = TargetActor->FindComponentByClass<UMeshComponent>();
			UE_LOG(LogTemp, Log, TEXT("AT_FIind : 감지된 너의 이름은? %s"), *Mesh->GetName());
			UE_LOG(LogTemp, Log, TEXT("AT_FIind : 소켓 이름 %s"), *TargetSocketNameToFind.ToString());
			if (Mesh && Mesh->DoesSocketExist(TargetSocketNameToFind))
			{
				// 목표 위치 업데이트
				TargetLocation = Mesh->GetSocketLocation(TargetSocketNameToFind);
				return true;
			}
		}
	}

	return false;
}
