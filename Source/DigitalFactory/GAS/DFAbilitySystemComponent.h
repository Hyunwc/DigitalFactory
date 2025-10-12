// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "DFAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCellStateChanged, FGameplayTag, NewState);
/**
 * 
 */
UCLASS()
class DIGITALFACTORY_API UDFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UDFAbilitySystemComponent();

public:
	//UPROPERTY(EditAnywhere, Category = "GAS")
	//FGameplayTagContainer CellStateTag;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnCellStateChanged OnCellStateChanged;

public:
	// 특정 상태 태그를 가지고 있는지 확인
	UFUNCTION(BlueprintCallable)
	bool HasCellState(FGameplayTag StateTag) const;

	// 셀 상태를 변경. 반환값은 성공 여부를
	UFUNCTION(BlueprintCallable)
	bool SetCellState(FGameplayTag NewStateTag);
	
	UFUNCTION(BlueprintCallable)
	void AddInitialCellStateTags(const FGameplayTagContainer& InitialTags);

	// 현재 셀 상태 반환
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentCellStateTag() const;
};
