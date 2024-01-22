// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGunFireAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Tags/CombatTags.h"
#include "Tags/GasTags.h"
#include "Utils/Constants.h"

UUpGunFireAbility::UUpGunFireAbility()
{
	AbilityTags.AddTag(TAG_Ability_GunFire);
}

void UUpGunFireAbility::OnGameplayTaskInitialized(UGameplayTask& Task)
{
	Super::OnGameplayTaskInitialized(Task);

	check(CooldownGameplayEffectClass);
	check(EffectClass_AmmoCost);
}

float UUpGunFireAbility::GetRepeatInterval() const
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Weapon = Character->GetActiveWeapon())
		{
			return Weapon->GetBurstShotInterval();
		}
	}

	return 0.1f;
}

void UUpGunFireAbility::HandleRepeatAction(const int32 ActionNumber)
{
	Super::HandleRepeatAction(ActionNumber);

	if (!CheckCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo())) return;
	
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Weapon = Character->GetActiveWeapon())
		{
			if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
			{
				bool bHasSufficientAmmo = false;

				if (const auto Effect = EffectClass_AmmoCost ? EffectClass_AmmoCost->GetDefaultObject<UGameplayEffect>() : nullptr)
				{
					// Manually do what `CheckCost()` does under the hood since we're checking attributes on a different actor.
					bHasSufficientAmmo = WeaponAbilitySystemComponent->CanApplyAttributeModifiers(Effect, GetAbilityLevel(),
						WeaponAbilitySystemComponent->MakeEffectContext());
				}
	
				if (bHasSufficientAmmo)
				{
					FVector ReticleWorldPosition;
					FVector ReticleWorldDirection;

					if (const auto CustomPlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
						CustomPlayerController && CustomPlayerController->ProjectReticleToWorld(ReticleWorldPosition, ReticleWorldDirection))
					{
						WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
							*WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass_AmmoCost, GetAbilityLevel(), WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get());

						if (const auto Montage = Character->GetGunFiringMontage())
						{
							const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
								this, NAME_None, Montage, 1.f, Weapon->GetMontageSectionName());
							PlayMontageAndWaitTask->Activate();
						}
				
						const auto LineTraceStart = ReticleWorldPosition;
						TArray<AActor*> IgnoredActors { Character };

						for (const auto Actor : Character->Children)
						{
							IgnoredActors.Add(Actor);
						}

						TArray<FHitResult> HitResults;
						UKismetSystemLibrary::LineTraceMulti(this, LineTraceStart, LineTraceStart + ReticleWorldDirection * Weapon->GetRange(),
							UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_WEAPON), false, IgnoredActors,
							bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, true);

						for (const auto& HitResult : HitResults)
						{
							if (bDebug)
							{
								UE_LOG(LogTemp, Warning, TEXT("Hit actor %s"), *HitResult.GetActor()->GetName())
							}
						}
			
						UE_LOG(LogTemp, Warning, TEXT("Shoot"))
					
						if (Weapon->GetFiringMode() == EUpWeaponFiringMode::SemiAutomatic)
						{
							ApplyCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
							EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
							return;
						}

						BurstShotCount++;

						if (bDebug)
						{
							UE_LOG(LogTemp, Warning, TEXT("Burst shot count: %d"), BurstShotCount)
						}

						if (BurstShotCount >= Weapon->GetBurstSize())
						{
							BurstShotCount = 0;
							ApplyCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
						}
						
						return;

						// 		if (HitResult.bBlockingHit)
						// 		{
						// 			if (const auto HitActor = HitResult.GetActor())
						// 			{
						// 				if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
						// 				{
						// 					auto EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
						// 					EffectContextHandle.AddHitResult(HitResult);
						// 					EffectContextHandle.AddOrigin(LineTraceStart);
						//
						// 					FGameplayEventData EventPayload;
						// 					EventPayload.ContextHandle = EffectContextHandle;
						//
						// 					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, TAG_Combat_HitReaction, EventPayload);
						// 				}
						//
						// 				TriggerDamage(HitActor);
						// 			}
						//
						// 			// if (ImpactParticleSystem) UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticleSystem, HitResult.Location);
						// 		}
					}
				} else
				{
					if (const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
					{
						FGameplayTagContainer AbilityTags;
						AbilityTags.AddTag(TAG_Combat_Reload);

						AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
					}

					return;
				}
			}
		}
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}
