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
					for (const auto AppReq : Effect->ApplicationRequirements)
					{
						const auto AppReqDefaultObject = AppReq ?
							AppReq->GetDefaultObject<UGameplayEffectCustomApplicationRequirement>() : nullptr;

						if (const auto EffectSpec = WeaponAbilitySystemComponent->MakeOutgoingSpec(
							EffectClass, GetAbilityLevel(), WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get();
								AppReqDefaultObject && EffectSpec && EffectSpec->Def)
						{
							if (!AppReqDefaultObject->CanApplyGameplayEffect(EffectSpec->Def, *EffectSpec, WeaponAbilitySystemComponent))
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
