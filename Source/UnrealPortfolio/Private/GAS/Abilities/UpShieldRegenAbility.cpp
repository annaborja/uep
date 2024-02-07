// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpShieldRegenAbility.h"

#include "AbilitySystemComponent.h"
#include "Tags/GasTags.h"

UUpShieldRegenAbility::UUpShieldRegenAbility()
{
	AbilityTags.AddTag(TAG_Ability_ShieldRegen);

	RepeatInterval = 0.01f;
}

void UUpShieldRegenAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);
	
	check(CooldownGameplayEffectClass);
	check(EffectClass);
}

void UUpShieldRegenAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CheckCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
	}
}

void UUpShieldRegenAbility::HandleRepeatAction(const int32 ActionNumber)
{
	Super::HandleRepeatAction(ActionNumber);

	if (CheckCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()))
	{
		if (const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo(); AbilitySystemComponent && EffectClass &&
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
				*AbilitySystemComponent->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), AbilitySystemComponent->MakeEffectContext()).Data.Get())
				.WasSuccessfullyApplied())
		{
			return;
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}
