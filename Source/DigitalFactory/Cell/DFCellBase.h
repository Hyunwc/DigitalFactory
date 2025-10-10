// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DFCellBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ATargetPoint;
class UAbilitySystemComponent;
class ADFAGV;

// 셀에서 작업이 완료되었음을 알리는 델리게이트 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCellWorkComplete, ADFCellBase*, CompleteCell);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API ADFCellBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADFCellBase();

protected:
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	// 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	// 외형
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	// AGV가 도달할 타겟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	ATargetPoint* AGVTargetPoint;

	// 어빌리티시스템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC;

	// 이 셀의 상태를 나타낼 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	FGameplayTagContainer CellTag;

	// 현재 셀에 예약된 AGV를 추척하기 위한 변수
	UPROPERTY(VisibleAnywhere)
	ADFAGV* ReservedAGV;

	// 현재 이 셀에서 작업중인 AGV
	UPROPERTY(VisibleAnywhere)
	ADFAGV* WorkingAGV;

	// 이 셀의 작업이 완료됐음을 알릴 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnCellWorkComplete OnCellWorkComplete;

public:
	// AGV에게 작업 시작을 요청받을 함수
	UFUNCTION(BlueprintCallable)
	virtual void StartWork(ADFAGV* TargetAGV);
};
