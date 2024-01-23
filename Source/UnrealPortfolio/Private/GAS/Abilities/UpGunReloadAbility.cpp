// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGunReloadAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Interfaces/UpCombatable.h"
#include "Items/UpWeapon.h"
#include "Tags/GasTags.h"

UUpGunReloadAbility::UUpGunReloadAbility()
{
	AbilityTags.AddTag(TAG_Ability_GunReload);
}

void UUpGunReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto AvatarActor = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Weapon = AvatarActor->GetActiveWeapon())
		{
			if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
			{
				bool bCanReload = true;

				if (EffectClass)
				{
					bCanReload = WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
						*WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get()
						).WasSuccessfullyApplied();
				}
				
				if (bCanReload)
				{
					if (const auto Combatable = Cast<IUpCombatable>(AvatarActor))
					{
						if (const auto Montage = Combatable->GetReloadsMontage())
						{
							const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
								this, NAME_None, Montage, 1.f, GetReloadsMontageSectionName());
							PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
							PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
							PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageInterrupted);
							PlayMontageAndWaitTask->Activate();

							return;
						}
					}
				}
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UUpGunReloadAbility::OnMontageCompleted()
{
	if (EffectClass_Success)
	{
		if (const auto AvatarActor = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
		{
			if (const auto Weapon = AvatarActor->GetActiveWeapon())
			{
				if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
				{
					WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
						*WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass_Success, GetAbilityLevel(), WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get());
				}
			}
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UUpGunReloadAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

FName UUpGunReloadAbility::GetReloadsMontageSectionName() const
{
	return NAME_None;
}
