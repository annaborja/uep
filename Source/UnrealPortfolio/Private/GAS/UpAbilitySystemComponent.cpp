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
		ApplyGameplayEffectSpecToSelf(*MakeOutgoingSpec(
			EffectClass, Combatable ? Combatable->GetCombatLevel() : 1.f, MakeEffectContext()).Data.Get());
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
			for (const auto AbilityClass : GasDataAsset->GetGrantedAbilityClasses_Common())
			{
				GrantAbility(AbilityClass);
			}
		}
	}
}

void UUpAbilitySystemComponent::GrantAbility(const TSubclassOf<UGameplayAbility> AbilityClass)
{
	const auto Combatable = Cast<IUpCombatable>(GetAvatarActor());
	
	GiveAbility(FGameplayAbilitySpec(AbilityClass, Combatable ? Combatable->GetCombatLevel() : 1));
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

void UUpAbilitySystemComponent::RevokeAbility(const TSubclassOf<UGameplayAbility> AbilityClass)
{
	FGameplayAbilitySpecHandle SpecHandle;
	
	for (const auto& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability.GetClass() == AbilityClass)
		{
			SpecHandle = AbilitySpec.Handle;
			break;
		}
	}

	if (SpecHandle.IsValid()) ClearAbility(SpecHandle);
}
