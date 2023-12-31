// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpGunFireAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectCustomApplicationRequirement.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Tags/CombatTags.h"
#include "Utils/Constants.h"

UUpGunFireAbility::UUpGunFireAbility()
{
	AbilityTags.AddTag(TAG_Combat_Attack_Gun_Fire);
}

void UUpGunFireAbility::HandleRepeatAction(int32 ActionNumber)
{
	Super::HandleRepeatAction(ActionNumber);

	if (const auto AvatarActor = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto& WeaponSlotData = AvatarActor->GetCharacterEquipment().GetPotentialActiveWeaponSlotData(); WeaponSlotData.bActivated)
		{
			if (const auto Weapon = Cast<AUpWeapon>(WeaponSlotData.ItemInstance.ItemActor))
			{
				bool bHasSufficientAmmo = true;

				if (AmmoCostEffectClass)
				{
					if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
					{
						bHasSufficientAmmo = WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
							*WeaponAbilitySystemComponent->MakeOutgoingSpec(AmmoCostEffectClass, GetAbilityLevel(), WeaponAbilitySystemComponent->MakeEffectContext()).Data.Get()
								).WasSuccessfullyApplied();
					}
				}
			
				if (bHasSufficientAmmo)
				{
					if (const auto World = GetWorld())
					{
						FVector2D ViewportSize;
						if (GEngine && GEngine->GameViewport) GEngine->GameViewport->GetViewportSize(ViewportSize);

						FVector CrosshairWorldPosition;
						FVector CrosshairWorldDirection;

						if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
							PlayerController && UGameplayStatics::DeprojectScreenToWorld(PlayerController,
							FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f), CrosshairWorldPosition, CrosshairWorldDirection))
						{
							const auto LineTraceStart = CrosshairWorldPosition;
							const auto LineTraceEnd = CrosshairWorldPosition + CrosshairWorldDirection * LineTraceLength;

							FCollisionQueryParams LineTraceParams;
							LineTraceParams.AddIgnoredActor(AvatarActor);
							LineTraceParams.AddIgnoredActors(AvatarActor->Children);

							FHitResult HitResult;
							World->LineTraceSingleByChannel(HitResult, LineTraceStart, LineTraceEnd, TRACE_CHANNEL_WEAPON, LineTraceParams);

							if (bDebug)
							{
								UKismetSystemLibrary::DrawDebugLine(this, LineTraceStart, LineTraceEnd, FColor::Cyan, 1.f);
							}

							if (HitResult.bBlockingHit)
							{
								if (const auto HitActor = HitResult.GetActor())
								{
									if (bDebug)
									{
										UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s, component: %s"), *HitActor->GetName(), *HitResult.GetComponent()->GetName())
									}

									if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
									{
										auto EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
										EffectContextHandle.AddHitResult(HitResult);
										EffectContextHandle.AddOrigin(LineTraceStart);

										FGameplayEventData EventPayload;
										EventPayload.ContextHandle = EffectContextHandle;

										UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, TAG_Combat_HitReaction, EventPayload);
									}

									TriggerDamage(HitActor);
								}

								if (ImpactParticleSystem) UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticleSystem, HitResult.Location);
							}
						}
					}
				} else
				{
					if (const auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
					{
						FGameplayTagContainer AbilityTags;
						AbilityTags.AddTag(TAG_Combat_Reload);
			
						AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
					}
				}
			}
		}
	}
}
