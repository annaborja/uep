// Copyright AB. All Rights Reserved.

#include "GAS/UpAbilitySystemComponent.h"

#include "UpGameInstance.h"
#include "GAS/UpGasDataAsset.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "Interfaces/UpCombatable.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpAbilitySystemComponent::Init(AActor* InOwnerActor, AActor* InAvatarActor,
	const TArray<TSubclassOf<UGameplayEffect>>& InitAttributesEffectClasses, const TArray<TSubclassOf<UGameplayAbility>>& GrantedAbilityClasses)
{
	InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	const auto Combatable = Cast<IUpCombatable>(InAvatarActor);

	for (const auto EffectClass : InitAttributesEffectClasses)
	{
		ApplyGameplayEffectSpecToTarget(*MakeOutgoingSpec(
			EffectClass, Combatable ? Combatable->GetCombatLevel() : 1.f, MakeEffectContext()).Data.Get(), this);
	}

	if (InAvatarActor->HasAuthority())
	{
		for (const auto AbilityClass : GrantedAbilityClasses)
		{
			GrantAbility(AbilityClass);
		}
	}

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
		{
			for (const auto AbilityClass : GasDataAsset->GetCommonGrantedAbilityClasses())
			{
				GrantAbility(AbilityClass);
			}
		}
	}
}

void UUpAbilitySystemComponent::GrantAbility(const TSubclassOf<UGameplayAbility> AbilityClass)
{
	const auto Combatable = Cast<IUpCombatable>(GetAvatarActor());
	auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, Combatable ? Combatable->GetCombatLevel() : 1);

	if (const auto Ability = Cast<UUpGameplayAbility>(AbilitySpec.Ability))
	{
		if (const auto& TagId = Ability->GetTagId(); TagId.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(TagId);
		}
	}
	
	GiveAbility(AbilitySpec);
}

bool UUpAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& Tag)
{
	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			return TryActivateAbility(AbilitySpec.Handle);
		}
	}

	return false;
}
