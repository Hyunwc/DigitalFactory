// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DFPlayerController.h"
#include "Camera/DFCameraActor.h"
#include "Kismet/GameplayStatics.h"

ADFPlayerController::ADFPlayerController()
{
	bShowMouseCursor = true;
}

void ADFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(this);

	SetInputMode(FInputModeGameAndUI());

	// 월드에 있는 카메라 액터를 전부 찾아온다
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraActorClass, FoundCameras);

	// 순회하면서 CameraMap에 추가한다.
	for (AActor* Actor : FoundCameras)
	{
		ADFCameraActor* Camera = Cast<ADFCameraActor>(Actor);

		CameraMap.Add(Camera->CameraTag, Camera);
	}

	//SwitchCamera(TEXT("Main"));
	SetViewTarget(CameraMap.FindRef(TEXT("Main")));
}

void ADFPlayerController::SwitchCamera(FName CameraTag)
{
	if (CameraMap.Num() == 0)
	{
		return;
	}

	// 넘겨 받은 키랑 일치하는 Value(카메라)를 찾아서 체인지
	SetViewTargetWithBlend(CameraMap.FindRef(CameraTag), 1.0f);

	// 시점이 바꼈다는 것을 구독자에게 알림
	OnCameraViewChanged.Broadcast(CameraMap.FindRef(CameraTag)->DisplayName);
}
