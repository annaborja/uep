// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpHitReactionAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Characters/UpPlayableCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/GasTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpHitReactionAbility::UUpHitReactionAbility()
{
	AbilityTags.AddTag(TAG_Ability_HitReaction);

	auto TriggerData = FAbilityTriggerData();
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TriggerData.TriggerTag = TAG_Ability_HitReaction;
	
	AbilityTriggers.Add(TriggerData);
}

void UUpHitReactionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto PlayableCharacter = Cast<AUpPlayableCharacter>(Character); !PlayableCharacter || !PlayableCharacter->IsPlayer())
		{
			if (const auto Montage = Character->GetHitReactionsMontage())
			{
				if (const auto HitResult = TriggerEventData->ContextHandle.GetHitResult())
				{
					const auto MontageSectionName = GetMontageSectionName(*HitResult);

					if (bDebug)
					{
						UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
					}
				
					UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
						this, NAME_None, Montage, 1.f, MontageSectionName)->Activate();
				}	
			}
		}
	}

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

FName UUpHitReactionAbility::GetMontageSectionName(const FHitResult& HitResult) const
{
	FString MontageSectionString = TEXT("");
	
	if (const auto Character = Cast<AUpCharacter>(HitResult.GetActor()))
	{
		MontageSectionString += UUpBlueprintFunctionLibrary::GetWeaponMontageSectionName(Character);
		MontageSectionString += TEXT(".");
	
		const auto CharacterForwardVector = Character->GetActorForwardVector();
		// We can't just use `HitResult.ImpactNormal` because the collision occurs on the character mesh,
		// which complicates the resulting impact normals.
		const auto ReverseHitPath = (-(HitResult.ImpactPoint - HitResult.TraceStart)).GetSafeNormal2D();

		if (bDebug)
		{
			const auto CharacterLocation = Character->GetActorLocation();
			
			UKismetSystemLibrary::DrawDebugArrow(this,
				HitResult.ImpactPoint, HitResult.ImpactPoint + ReverseHitPath * 100.0, 10.f, FColor::Red, 1.f);
			UKismetSystemLibrary::DrawDebugArrow(this,
				CharacterLocation, CharacterLocation + CharacterForwardVector * 100.0, 10.f, FColor::Green, 1.f);
		}
		
		auto Theta = FMath::RadiansToDegrees(FMath::Acos(CharacterForwardVector.Dot(ReverseHitPath)));

		if (FVector::CrossProduct(CharacterForwardVector, ReverseHitPath).Z < 0)
		{
			Theta *= -1.f;
		}

		if (Theta >= -45.f && Theta <= 45.f)
		{
			MontageSectionString += TEXT("Bwd");
		} else if (Theta < -45.f && Theta > -135.f)
		{
			MontageSectionString += TEXT("Rwd");
		} else if (Theta > 45.f && Theta < 135.f)
		{
			MontageSectionString += TEXT("Lwd");
		} else
		{
			MontageSectionString += TEXT("Fwd");
		}
	}

	return FName(MontageSectionString);
}
