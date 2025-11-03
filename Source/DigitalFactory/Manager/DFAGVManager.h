// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "DFAGVManager.generated.h"

class ADFAGV;
struct FDFOrderSpec;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIdleAGVCountDelegate, int32, IdleAGVCount);

UCLASS()
class DIGITALFACTORY_API ADFAGVManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFAGVManager();

protected:
	virtual void BeginPlay() override;

public:
	// AGV안에 태그가 있기 때문에 TSet으로 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV")
	TSet<ADFAGV*> AGVList;

	// 작업 타이머 핸들
	FTimerHandle WorkTimer; 

	// 간격
	UPROPERTY(EditAnywhere, Category = "AGV")
	float WorkDuration;

	UPROPERTY(BlueprintAssignable)
	FOnIdleAGVCountDelegate OnIdleAGVCount;

public:
	// 테스트용 함수
	UFUNCTION(CallInEditor, Category = "AGV")
	void CommandWork();

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void StartOrder(FDFOrderSpec Spec);

	// 델리게이트 전용
	UFUNCTION(BlueprintCallable, Category = "AGV")
	void RenewalIdleAGVCount();

	// 읽어오기 전용
	UFUNCTION(BlueprintCallable, Category = "AGV")
	int32 GetIdleAGVCount();
};
