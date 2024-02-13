// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpMeleeAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Tags/GasTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpMeleeAttackAbility::UUpMeleeAttackAbility()
{
	AbilityTags.AddTag(TAG_Ability_MeleeAttack);

	ActivationOwnedTags.AddTag(TAG_State_MeleeAttacking);
}

void UUpMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Montage = Character->GetGunMeleeAttacksMontage())
		{
			const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, Montage, 1.f, FName(UUpBlueprintFunctionLibrary::GetWeaponMontageSectionName(Character)));
			PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::TriggerAbilityEnd);
			PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::TriggerAbilityEnd);
			PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::TriggerAbilityEnd);
			PlayMontageAndWaitTask->Activate();
		}
	}
}

void UUpMeleeAttackAbility::TriggerAbilityEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
