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
	// Ư�� ���� �±׸� ������ �ִ��� Ȯ��
	UFUNCTION(BlueprintCallable)
	bool HasCellState(FGameplayTag StateTag) const;

	// �� ���¸� ����. ��ȯ���� ���� ���θ�
	UFUNCTION(BlueprintCallable)
	bool SetCellState(FGameplayTag NewStateTag);
	
	UFUNCTION(BlueprintCallable)
	void AddInitialCellStateTags(const FGameplayTagContainer& InitialTags);

	// ���� �� ���� ��ȯ
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentCellStateTag() const;
};
