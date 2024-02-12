// Copyright AB. All Rights Reserved.

#include "Components/UpCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "UpGameInstance.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/UpCharacter.h"
#include "GAS/UpGasDataAsset.h"
#include "Interfaces/UpCombatable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/GasTags.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpCombatComponent::UUpCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UUpCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AUpCharacter>(GetOwner());
}

void UUpCombatComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleActiveHitBoxes();
}

void UUpCombatComponent::HandleActiveHitBoxes()
{
	if (!Character) return;
	
	const UDataTable* HitBoxDataTable = nullptr;

	for (const auto& ActiveHitBoxName : Character->GetActiveHitBoxNames())
	{
		if (!HitBoxDataTable)
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
			{
				HitBoxDataTable = GameInstance->GetHitBoxDataTable();
			}
		}

		if (HitBoxDataTable) HandleActiveHitBox(ActiveHitBoxName, HitBoxDataTable);
	}
}

void UUpCombatComponent::HandleActiveHitBox(const FName& HitBoxName, const UDataTable* HitBoxDataTable)
{
	if (const auto HitBoxData = HitBoxDataTable->FindRow<FUpHitBoxData>(HitBoxName, TEXT("HitBoxData")))
	{
		if (const auto Owner = GetOwner(); Owner && Owner->HasAuthority())
		{
			const auto Origin = Owner->GetActorLocation();
			const auto Rotation = Owner->GetActorRotation();

			TArray<AActor*> IgnoredActors { Owner };
			
			for (const auto Actor : Owner->Children)
			{
				IgnoredActors.Add(Actor);
			}

			TArray<FHitResult> OutHits;
			UKismetSystemLibrary::SphereTraceMulti(this,
				Origin + Rotation.RotateVector(HitBoxData->StartLocationOffset),
				Origin + Rotation.RotateVector(HitBoxData->EndLocationOffset),
				HitBoxData->HalfSize, UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_WEAPON), false,
				IgnoredActors, bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, OutHits, true);

			for (const auto& OutHit : OutHits)
			{
				HandleHitResult(OutHit);
			}
		}
	}
}

void UUpCombatComponent::HandleHitResult(const FHitResult HitResult)
{
	if (!Character) return;
	
	if (const auto HitActor = HitResult.GetActor(); HitActor && !Character->GetHitActors().Contains(HitActor))
	{
		Character->GetHitActors().Add(HitActor);

		if (const auto TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
		{
			auto EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddHitResult(HitResult);

			FGameplayEventData EventPayload;
			EventPayload.ContextHandle = EffectContextHandle;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, TAG_Ability_HitReaction, EventPayload);

			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(HitActor))
			{
				if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
				{
					if (const auto EffectClass = GasDataAsset->GetEffectClass_Damage())
					{
						if (const auto Owner = GetOwner())
						{
							if (const auto AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
							{
								const auto Combatable = Cast<IUpCombatable>(Owner);
								
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
