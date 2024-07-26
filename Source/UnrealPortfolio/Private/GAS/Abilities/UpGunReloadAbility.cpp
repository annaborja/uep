// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGunReloadAbility.h"

#include "GameplayEffectCustomApplicationRequirement.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Tags/GasTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpGunReloadAbility::UUpGunReloadAbility()
{
	AbilityTags.AddTag(TAG_Ability_GunReload);

	ActivationBlockedTags.AddTag(TAG_Ability_GunFire);
}

void UUpGunReloadAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);

	check(EffectClass);
}

void UUpGunReloadAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Weapon = Character->GetActiveWeapon())
		{
			if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
			{
				bool bCanReload = true;

				if (const auto Effect = EffectClass ? EffectClass->GetDefaultObject<UGameplayEffect>() : nullptr)
				{
					// This code is based on code in the engine in an attempt to avoid a situation where the game
					// would randomly crash while executing this ability (maybe due to something being garbage collected?).
					for (const auto& AppReq : Effect->ApplicationRequirements_DEPRECATED)
					{
						if (*AppReq)
						{
							if (!AppReq->GetDefaultObject<UGameplayEffectCustomApplicationRequirement>()->
								CanApplyGameplayEffect(Effect, *WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass,
									GetAbilityLevel(), WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get(), WeaponAbilitySystemComponent))
							{
								bCanReload = false;
								break;
							}
						} else
						{
							bCanReload = false;
							break;
						}
					}
				} else
				{
					bCanReload = false;
				}
				
				if (bCanReload)
				{
					if (const auto Montage = Character->GetReloadsMontage())
					{
						const auto MontageSectionName = FName(UUpBlueprintFunctionLibrary::GetWeaponMontageSectionName(Character));

						if (bDebug)
						{
							UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
						}
						
						const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this, NAME_None, Montage, 1.f, MontageSectionName);
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

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UUpGunReloadAbility::OnMontageCompleted()
{
	if (EffectClass)
	{
		if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
		{
			if (const auto Weapon = Character->GetActiveWeapon())
			{
				if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
				{
					WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
						*WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass, GetAbilityLevel(),
							WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get());
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
