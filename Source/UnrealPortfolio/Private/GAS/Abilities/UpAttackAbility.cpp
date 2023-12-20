// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpAttackAbility.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"

void UUpAttackAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);
	
	OnRepeatDelegate.BindUFunction(this, FName("HandleRepeatAction"));
}

void UUpAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (RepeatInterval > 0.f)
	{
		const auto RepeatTask = UAbilityTask_Repeat::RepeatAction(this, RepeatInterval, MAX_int32);
		RepeatTask->OnPerformAction.Add(OnRepeatDelegate);
		RepeatTask->Activate();
	}
}

void UUpAttackAbility::HandleRepeatAction(int32 ActionNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("pew"))
}
