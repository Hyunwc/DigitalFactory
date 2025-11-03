// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DFPlayerController.generated.h"

class ADFCameraActor;
// 카메라 시점이 변경되었음을 알리는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraViewChanged, FText, NewDisplay);

/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADFPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<ADFCameraActor> CameraActorClass;

	// Map으로 관리하는 이유 : 매번 Array로 찾아서 순회하는 방법보다 Key(카메라 이름)값으로 찾는 것이 훨씬 빠를 것이라 생각되기 때문
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TMap<FName, ADFCameraActor*> CameraMap;

	UPROPERTY(EditAnywhere, Category = "Camera")
	ADFCameraActor* CurrentCamera;

	UPROPERTY(BlueprintAssignable)
	FOnCameraViewChanged OnCameraViewChanged;

public:
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SwitchCamera(FName CameraTag);
};
