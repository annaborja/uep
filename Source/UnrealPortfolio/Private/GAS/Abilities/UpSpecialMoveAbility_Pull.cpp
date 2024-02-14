// Copyright AB. All Rights Reserved.

#include "GAS/Abilities/UpSpecialMoveAbility_Pull.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/UpCharacter.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/GasTags.h"
#include "Tags/ScriptTags.h"
#include "Utils/Constants.h"

UUpSpecialMoveAbility_Pull::UUpSpecialMoveAbility_Pull()
{
	AbilityTags.AddTag(TAG_Ability_SpecialMove_Pull);
}

void UUpSpecialMoveAbility_Pull::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const auto Montage = Character->GetSpecialMovesMontage())
		{
			const auto PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, Montage, 1.f, FName(TEXT("Pull")));
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

void UUpSpecialMoveAbility_Pull::OnGameplayEventReceived(FGameplayEventData Payload)
{
	if (const auto Character = Cast<AUpCharacter>(GetAvatarActorFromActorInfo()))
	{
		const auto TraceStart = Character->GetActorLocation();
		
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(Character);

		for (const auto Actor : Character->Children)
		{
			IgnoredActors.Add(Actor);
		}
		
		TArray<FHitResult> OutHits;
		UKismetSystemLibrary::CapsuleTraceMulti(Character, TraceStart, TraceStart + Character->GetActorForwardVector() * 10000.f,
			50.f, 5.f, UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_WEAPON), false, IgnoredActors,
			bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, OutHits, true);

		for (const auto& Hit : OutHits)
		{
			if (const auto HitActor = Cast<AUpCharacter>(Hit.GetActor()))
			{
				if (bDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitActor->GetName())
				}

				if (const auto CustomMovementComponent = HitActor->GetCustomMovementComponent())
				{
					CustomMovementComponent->StopMovementImmediately();
					CustomMovementComponent->AddImpulse(
						(-Character->GetActorForwardVector() + FVector(0.f, 0.f, 0.5f)).GetSafeNormal() *
						FMath::Min(1000.f, ((HitActor->GetActorLocation() - Character->GetActorLocation()).Size())), true);
					HitActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), HitActor->GetActorLocation()));
				}
			}
		}
	}
	
	WaitGameplayEventTask->EndTask();
}

void UUpSpecialMoveAbility_Pull::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
