// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/DFAbilitySystemComponent.h"

UDFAbilitySystemComponent::UDFAbilitySystemComponent()
{
}

bool UDFAbilitySystemComponent::HasCellState(FGameplayTag StateTag) const
{
	if (!StateTag.IsValid())
	{
		return false;
	}

	return HasMatchingGameplayTag(StateTag);
}

bool UDFAbilitySystemComponent::SetCellState(FGameplayTag NewStateTag)
{
	if (!NewStateTag.IsValid())
	{
		return false;
	}
	
	// 현재 존재하는 태그 모두 제거
	RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));
	RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Cell.State.Working"));

	// 새로운 상태 추가
	AddLooseGameplayTag(NewStateTag);

	OnCellStateChanged.Broadcast(NewStateTag);
	return true;
}

void UDFAbilitySystemComponent::AddInitialCellStateTags(const FGameplayTagContainer& InitialTags)
{
	for (const FGameplayTag& Tag : InitialTags)
	{
		if (Tag.IsValid())
		{
			AddLooseGameplayTag(Tag);
		}
	}
}

FGameplayTag UDFAbilitySystemComponent::GetCurrentCellStateTag() const
{
	if (HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Working")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.State.Working");
	}
	if (HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Pending")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.State.Pending");
	}
	if (HasCellState(FGameplayTag::RequestGameplayTag("Cell.State.Free")))
	{
		return FGameplayTag::RequestGameplayTag("Cell.State.Free");
	}
	
	return FGameplayTag();
}
