// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpHitReactionAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Characters/UpCharacter.h"
#include "Interfaces/UpCombatable.h"
#include "Kismet/KismetSystemLibrary.h"

void UUpHitReactionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData)
	{
		if (const auto Combatable = Cast<IUpCombatable>(GetAvatarActorFromActorInfo()))
		{
			if (const auto HitReactionsMontage = Combatable->GetHitReactionsMontage())
			{
				if (const auto HitResult = TriggerEventData->ContextHandle.GetHitResult(); HitResult && TriggerEventData->ContextHandle.HasOrigin())
				{
					const auto MontageSectionName = GetHitReactionsMontageSectionName(TriggerEventData->ContextHandle.GetOrigin(), *HitResult);

					if (bDebug)
					{
						UE_LOG(LogTemp, Warning, TEXT("Montage section name: %s"), *MontageSectionName.ToString())
					}
				
					const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
						this, NAME_None, HitReactionsMontage, 1.f, MontageSectionName);
					PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageEnd);
					PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageEnd);
					PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageEnd);
					PlayMontageAndWaitTask->Activate();

					return;
				}
			}
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UUpHitReactionAbility::OnMontageEnd()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

FName UUpHitReactionAbility::GetHitReactionsMontageSectionName(const FVector& Origin, const FHitResult& HitResult) const
{
	FString MontageSectionString = "";
	
	if (const auto HitActor = HitResult.GetActor())
	{
		if (const auto Character = Cast<AUpCharacter>(HitActor))
		{
			switch (Character->GetPosture())
			{
			case EUpCharacterPosture::ArmedPistol:
				MontageSectionString += "Pistol";
				break;
			case EUpCharacterPosture::ArmedRevolver:
				MontageSectionString += "Revolver";
				break;
			case EUpCharacterPosture::ArmedRifle:
				MontageSectionString += "Rifle";
				break;
			default:
				MontageSectionString += "Unarmed";
				break;
			}
		}
		
		const auto ForwardVector = HitActor->GetActorForwardVector();
		// We can't just use `HitResult.ImpactNormal` because the collision occurs on the character mesh,
		// which complicates the resulting impact normals.
		const auto ReverseAttackPath = ((HitResult.ImpactPoint - Origin) * -1).GetSafeNormal2D();

		if (bDebug)
		{
			const auto ActorLocation = HitActor->GetActorLocation();
			
			UKismetSystemLibrary::DrawDebugArrow(this,
				HitResult.ImpactPoint, HitResult.ImpactPoint + ReverseAttackPath * 100.0, 10.f, FColor::Red, 1.f);
			UKismetSystemLibrary::DrawDebugArrow(this,
				ActorLocation, ActorLocation + ForwardVector * 100.0, 10.f, FColor::Green, 1.f);
		}
		
		auto Theta = FMath::RadiansToDegrees(FMath::Acos(ForwardVector.Dot(ReverseAttackPath)));

		if (FVector::CrossProduct(ForwardVector, ReverseAttackPath).Z < 0)
		{
			Theta *= -1.f;
		}

		if (Theta >= -45.f && Theta <= 45.f)
		{
			MontageSectionString += "Backward";
		} else if (Theta < -45.f && Theta > -135.f)
		{
			MontageSectionString += "Rightward";
		} else if (Theta > 45.f && Theta < 135.f)
		{
			MontageSectionString += "Leftward";
		} else
		{
			MontageSectionString += "Forward";
		}
	}

	return FName(MontageSectionString);
}
