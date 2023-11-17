// Copyright AB. All Rights Reserved.

#include "GAS/UpAbilitySystemComponent.h"

#include "GAS/UpGasDataAsset.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "Interfaces/UpCombatable.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"

void UUpAbilitySystemComponent::Init(AActor* InOwnerActor, AActor* InAvatarActor)
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
		for (const auto AbilityClass : StartupAbilityClasses)
		{
			GrantAbility(AbilityClass);
		}
	}

	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (const auto GasDataAsset = GameMode->GetGasDataAsset())
		{
			for (const auto AbilityClass : GasDataAsset->GetCommonStartupAbilityClasses())
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
