// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGunFireAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
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

void UUpGunFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	ResetBurstShotCount();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
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
					if (EffectClass_AmmoCost)
					{
						WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
							*WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass_AmmoCost, GetAbilityLevel(),
								WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get());
					}

					if (const auto Montage = Character->GetGunFiringMontage())
					{
						UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this, NAME_None, Montage, 1.f, Weapon->GetMontageSectionName())->Activate();
					}
					
					FVector ReticleWorldPosition;
					FVector ReticleWorldDirection;

					if (const auto CustomPlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
						CustomPlayerController && CustomPlayerController->ProjectReticleToWorld(ReticleWorldPosition, ReticleWorldDirection))
					{
						if (const auto WeaponMesh = Weapon->GetStaticMeshComponent())
						{
							const auto ReticleTraceStart = ReticleWorldPosition;
							TArray<AActor*> IgnoredActors { Character };

							for (const auto Actor : Character->Children)
							{
								IgnoredActors.Add(Actor);
							}

							TArray<FHitResult> ReticleHitResults;
							UKismetSystemLibrary::LineTraceMulti(this,
								ReticleTraceStart, ReticleTraceStart + ReticleWorldDirection * Weapon->GetRange(),
								UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_WEAPON), false, IgnoredActors,
								EDrawDebugTrace::None, ReticleHitResults, true);

							if (ReticleHitResults.Num() > 0)
							{
								TArray<FHitResult> MuzzleHitResults;
								UKismetSystemLibrary::LineTraceMulti(this,
									WeaponMesh->GetSocketLocation(FName(SOCKET_NAME_ATTACK_SOURCE)), ReticleHitResults.Last().ImpactPoint,
									UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_WEAPON), false, IgnoredActors,
									bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, MuzzleHitResults, true);

								for (const auto& MuzzleHitResult : MuzzleHitResults)
								{
									if (const auto HitActor = MuzzleHitResult.GetActor())
									{
										if (bDebug)
										{
											UE_LOG(LogTemp, Warning, TEXT("Hit actor %s"), *HitActor->GetName())
										}
										
										if (MuzzleHitResult.bBlockingHit)
										{
											if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
											{
												FGameplayCueParameters CueParams;
												CueParams.Location = MuzzleHitResult.ImpactPoint;
												CueParams.Normal = MuzzleHitResult.ImpactNormal;
												CueParams.SourceObject = Weapon;

												TargetAbilitySystemComponent->ExecuteGameplayCue(TAG_GameplayCue_GunFire_Impact, CueParams);
												
												auto EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
												EffectContextHandle.AddHitResult(MuzzleHitResult);
			
												FGameplayEventData EventPayload;
												EventPayload.ContextHandle = EffectContextHandle;
			
												UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, TAG_Ability_HitReaction, EventPayload);
											}
			
											TriggerDamage(MuzzleHitResult);
										}
									}
								}
							}
						}
					}
			
					if (Weapon->GetFiringMode() == EUpWeaponFiringMode::SemiAutomatic)
					{
						ApplyCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
						EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
						return;
					}

					BurstShotCount++;

					if (BurstShotCount >= Weapon->GetBurstSize())
					{
						ResetBurstShotCount();
						ApplyCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
					}
					
					return;
				}
				
				if (const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
				{
					FGameplayTagContainer AbilityTags;
					AbilityTags.AddTag(TAG_Ability_GunReload);

					AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
				}

				return;
			}
		}
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, true);
}

void UUpGunFireAbility::ResetBurstShotCount()
{
	BurstShotCount = 0;
}
