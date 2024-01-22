// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"

UUpGameplayAbility::UUpGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUpGameplayAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);

	OnRepeatDelegate.BindUFunction(this, FName("HandleRepeatAction"));
}

void UUpGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// We purposely don't call the Super method because it calls `CommitAbility()`, which we want to control ourselves.
	// Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start ability %s"), *GetName())
	}

	if (const auto DynamicRepeatInterval = GetRepeatInterval(); DynamicRepeatInterval > 0.f)
	{
		RepeatTask = UAbilityTask_Repeat::RepeatAction(this, DynamicRepeatInterval, GetRepeatMaxActionCount());
		RepeatTask->OnPerformAction.Add(OnRepeatDelegate);
		RepeatTask->Activate();
	}
}

void UUpGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("End ability %s"), *GetName())
	}

	if (RepeatTask) RepeatTask->EndTask();
}

void UUpGameplayAbility::TriggerDamage(AActor* TargetActor) const
{
	if (!DamageEffectClass) return;
	
	if (const auto SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
	{
		if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext()).Data.Get(),
				TargetAbilitySystemComponent);
		}
	}
}
