// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_FindAndRotateToTarget.h"
#include "GAS/GA/DFGA_TireAssembly.h"
#include "Robot/DFRobotArm.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Components/SphereComponent.h"
#include "ControlRigComponent.h"
#include "Components/MeshComponent.h"
#include "DrawDebugHelpers.h"

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
	FName EndControlName = FName("Robot1_End_Ctrl");
	// 하체 역할을 하는 A와 이펙터 역할을 하는 End의 컨트롤릭 트랜스폼 계산.
	FTransform AControlTransform = ControlRigComponent->GetControlTransform(TargetControlName, EControlRigComponentSpace::WorldSpace);
	FTransform EndControlTransform = ControlRigComponent->GetControlTransform(EndControlName, EControlRigComponentSpace::WorldSpace);
	
	StartRotation = AControlTransform.GetRotation().Rotator();
	// EndCtrl이 타겟을 바라볼 때 필요한 회전값 계산
	FVector EndControlLocation = EndControlTransform.GetLocation();
	FRotator LookAt_End = UKismetMathLibrary::FindLookAtRotation(EndControlLocation, TargetLocation);
	
	FRotator Current_A = StartRotation;
	// End와 A간의 차이를 계산 : 이 차이는 A가 현재 방향에서 LooAt 방향으로 추가로 얼마나 회전해야 하는지를
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(LookAt_End, Current_A);
	TargetRotation = FRotator(StartRotation.Pitch, LookAt_End.Yaw + Delta.Yaw, StartRotation.Roll);

	UE_LOG(LogTemp, Warning, TEXT("AT_FIind : Current_A X : %f, Y : %f, Z : %f"), Current_A.Pitch, Current_A.Yaw, Current_A.Roll);
	UE_LOG(LogTemp, Warning, TEXT("AT_FIind : LookAt_End X : %f, Y : %f, Z : %f"), LookAt_End.Pitch, LookAt_End.Yaw, LookAt_End.Roll);
	UE_LOG(LogTemp, Warning, TEXT("AT_FIind : Delta X : %f, Y : %f, Z : %f"), Delta.Pitch, Delta.Yaw, Delta.Roll);
	UE_LOG(LogTemp, Warning, TEXT("AT_FIind : TargetRotation X : %f, Y : %f, Z : %f"), TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll);
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
	//UE_LOG(LogTemp, Log, TEXT("AT_FIind : 타겟이 유효하니 여기 들어왔지요~"));
	if (LerpAlpha >= 1.0f)
	{
		// 1.0f 도달 : 최종 회전 설정 및 Task 종료
		NewRotation = TargetRotation;

		OnFindAndRotateFinished.Broadcast();
		EndTask();
	}
	else
	{
		//NewRotation = FMath::RInterpTo(StartRotation, TargetRotation, LerpAlpha, 1.0f);
		NewRotation = FMath::Lerp(StartRotation, TargetRotation, LerpAlpha);
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
		return false;
	}

	USphereComponent* SearchSphere = OwnerActor->FindComponentByClass<USphereComponent>();
	if (!SearchSphere)
	{
		return false;
	}

	TArray<AActor*> OverlappingActors;
	SearchSphere->GetOverlappingActors(OverlappingActors);

	if (OverlappingActors.Num() == 0)
	{
		return false;
	}

	// 순회해서 TargetSocket을 가진 액터 찾는다.
	for (AActor* TargetActor : OverlappingActors)
	{
		UE_LOG(LogTemp, Log, TEXT("AT_FIind : 오버랩에 들어있는 너의 이름은? %s"), *TargetActor->GetName());
		if (TargetActor)
		{
			UMeshComponent* Mesh = TargetActor->FindComponentByClass<UMeshComponent>();
			if (Mesh && Mesh->DoesSocketExist(TargetSocketNameToFind))
			{
				// 목표 위치 업데이트
				//TargetLocation = Mesh->GetSocketLocation(TargetSocketNameToFind);
				TargetLocation = TargetActor->GetActorLocation();
				
				return true;
			}
		}
	}

	return false;
}
