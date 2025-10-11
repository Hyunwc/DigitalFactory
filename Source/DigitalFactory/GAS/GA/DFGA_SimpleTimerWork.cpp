// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/DFGA_SimpleTimerWork.h"
#include "Cell/DFCellBase.h"
#include "AbilitySystemComponent.h"
#include "TimerManager.h" 

UDFGA_SimpleTimerWork::UDFGA_SimpleTimerWork()
{
	WorkDuration = 3.0f;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; // 액터당 인스턴스 하나

	WorkStartingTagsToAdd.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Working"));
	WorkStartingTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	WorkStartingTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Pending"));

	WorkEndingTagsToAdd.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Free"));
	WorkEndingTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag("Cell.State.Working"));
}

void UDFGA_SimpleTimerWork::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 어빌리티 활성화 전 커밋 (비용/쿨다운 체크, 실패 시 바로 종료)
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 어빌리티를 발동시킨 셀 액터 참조
	OwningCell = Cast<ADFCellBase>(ActorInfo->OwnerActor.Get());
	if (OwningCell && OwningCell->GetAbilitySystemComponent())
	{
		// 작업 시작 태그 적용
		OwningCell->GetAbilitySystemComponent()->RemoveLooseGameplayTags(WorkStartingTagsToRemove);
		OwningCell->GetAbilitySystemComponent()->AddLooseGameplayTags(WorkStartingTagsToAdd);
	}

	UE_LOG(LogTemp, Log, TEXT("[%s] Work Started on %s. Will Completre in %f seconds"),
		*GetName(), *OwningCell->GetName(), WorkDuration);

	// 작업 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(WorkTimerHandle, this, &UDFGA_SimpleTimerWork::OnWorkCompleted, WorkDuration, false);
}

void UDFGA_SimpleTimerWork::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 이미 종료되었으면 다시 처리 x
	if (!IsActive())
	{
		return;
	}

	// 작업 종료 태그 적용
	if (OwningCell && OwningCell->GetAbilitySystemComponent())
	{
		// 작업 시작 태그 적용
		OwningCell->GetAbilitySystemComponent()->RemoveLooseGameplayTags(WorkEndingTagsToRemove);
		OwningCell->GetAbilitySystemComponent()->AddLooseGameplayTags(WorkEndingTagsToAdd);

		// 작업이 완료되었음을 델리게이트를 통해 알림
		OwningCell->OnCellWorkComplete.Broadcast(OwningCell);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDFGA_SimpleTimerWork::OnWorkCompleted()
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Work Completed On %s!"), *GetName(), *OwningCell->GetName());

	// 타이머 정리
	GetWorld()->GetTimerManager().ClearTimer(WorkTimerHandle);

	// 작업 완료 후 어빌리티 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
