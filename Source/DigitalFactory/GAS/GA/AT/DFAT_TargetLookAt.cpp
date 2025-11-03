// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/AT/DFAT_TargetLookAt.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

UDFAT_TargetLookAt::UDFAT_TargetLookAt(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bTickingTask = false;
}

UDFAT_TargetLookAt* UDFAT_TargetLookAt::StartLookAt(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector TargetLocation, UStaticMeshComponent* Actor, float Duration)
{
    UDFAT_TargetLookAt* MyObj = NewAbilityTask<UDFAT_TargetLookAt>(OwningAbility, TaskInstanceName);

    if (MyObj)
    {
        MyObj->TargetLocation = TargetLocation;
        MyObj->Looker = Actor;
        MyObj->Duration = FMath::Max(Duration, 0.001f);
    }

    return MyObj;
}

void UDFAT_TargetLookAt::Activate()
{
    ElapsedTime = 0.0f;
    bTickingTask = true;
}

void UDFAT_TargetLookAt::TickTask(float DeltaTime)
{
    Super::TickTask(DeltaTime);

    if (!Looker || !Looker->IsVisible())
    {
        EndTask();
        return;
    }

    ElapsedTime += DeltaTime;
    float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

    FVector CurrentLocation = Looker->GetComponentLocation();
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
    FRotator CurrentRotation = Looker->GetComponentRotation();

    FRotator NewRotation = FMath::RInterpTo(
        CurrentRotation,
        TargetRotation,
        DeltaTime,
        Alpha
    );

    Looker->SetWorldRotation(NewRotation);

    if (Alpha >= 1.0f)
    {
        // 델리게이트는 필요 없음. 실제 데이터 이동이 아니라 시각적 연출을 위함이기 때문->어차피 결합된다면 비활성화될 것
        bTickingTask = false;
        EndTask();
    }
}

void UDFAT_TargetLookAt::OnDestroy(bool AbilityIsEnding)
{
    bTickingTask = false;
    Super::OnDestroy(AbilityIsEnding);
}
