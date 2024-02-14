// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSpecialMoveAbility_Aoe.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "UpGameInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/UpCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GAS/UpGasDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/GasTags.h"
#include "Tags/ScriptTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpSpecialMoveAbility_Aoe::UUpSpecialMoveAbility_Aoe()
{
	AbilityTags.AddTag(TAG_Ability_SpecialMove_Aoe);
}

void UUpSpecialMoveAbility_Aoe::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Montage = Character->GetSpecialMovesMontage())
		{
			const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, Montage, 1.f, FName(TEXT("AOE")));
			PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageCompleted);
			PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageCompleted);
			PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCompleted);
			PlayMontageAndWaitTask->Activate();

			if (WaitGameplayEventTask) WaitGameplayEventTask->EndTask();
				
			WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_Event_SpecialMove_Execute);
			WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnGameplayEventReceived);
			WaitGameplayEventTask->Activate();

			return;
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UUpSpecialMoveAbility_Aoe::OnGameplayEventReceived(FGameplayEventData Payload)
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(Character);

		for (const auto Actor : Character->Children)
		{
			IgnoredActors.Add(Actor);
		}

		const auto SphereLocation = Character->GetActorLocation();
		constexpr auto SphereRadius = 500.f;
		
		TArray<AActor*> OutActors;
		UKismetSystemLibrary::SphereOverlapActors(Character, SphereLocation, SphereRadius,
			TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_Pawn) }, AUpCharacter::StaticClass(),
			IgnoredActors, OutActors);

		if (bDebug)
		{
			UKismetSystemLibrary::DrawDebugSphere(Character, SphereLocation, SphereRadius, 16, FColor::Orange, 3.f);
		}

		for (const auto Actor : OutActors)
		{
			if (const auto HitActor = Cast<AUpCharacter>(Actor))
			{
				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *Actor->GetName())
				}

				if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
				{
					const auto CharacterToHitActor = (HitActor->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
					const FHitResult HitResult(HitActor, HitActor->GetHitCapsule_Body(), HitActor->GetActorLocation(), -CharacterToHitActor);
					
					auto EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
					EffectContextHandle.AddHitResult(HitResult);

					FGameplayCueParameters CueParams;
					CueParams.EffectCauser = Character;
					CueParams.SourceObject = this;
					CueParams.EffectContext = EffectContextHandle;
					CueParams.Location = HitActor->GetActorLocation();
					CueParams.Normal = FVector::UpVector;

					if (const auto Capsule = HitActor->GetCapsuleComponent())
					{
						CueParams.Location += HitResult.ImpactNormal * Capsule->GetScaledCapsuleRadius() +
							FVector::UpVector * Capsule->GetScaledCapsuleHalfHeight() * 0.5f;
					}

					TargetAbilitySystemComponent->ExecuteGameplayCue(TAG_GameplayCue_SpecialMoveAoe_Impact, CueParams);

					if (const auto CustomMovementComponent = HitActor->GetCustomMovementComponent())
					{
						CustomMovementComponent->StopMovementImmediately();
						CustomMovementComponent->AddImpulse((CharacterToHitActor + FVector(0.f, 0.f, 0.5f)).GetSafeNormal() * 500.f, true);
						// HitActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), HitActor->GetActorLocation()));
					}
	
					FGameplayEventData EventPayload;
					EventPayload.ContextHandle = EffectContextHandle;
	
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, TAG_Ability_HitReaction, EventPayload);

					if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(HitActor))
					{
						if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
						{
							if (const auto EffectClass = GasDataAsset->GetEffectClass_Damage())
							{
								if (const auto AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character))
								{
									const auto Combatable = Cast<IUpCombatable>(Character);
							
									auto EffectContext = AbilitySystemComponent->MakeEffectContext();
									EffectContext.AddHitResult(HitResult);
			
									AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*AbilitySystemComponent->MakeOutgoingSpec(
										EffectClass, Combatable ? Combatable->GetCombatLevel() : 1.f, EffectContext).Data.Get(), TargetAbilitySystemComponent);
								}
							}
						}
					}
				}
			}
		}
	}
	
	WaitGameplayEventTask->EndTask();
}

void UUpSpecialMoveAbility_Aoe::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
