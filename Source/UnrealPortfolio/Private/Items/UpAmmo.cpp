// Copyright AB. All Rights Reserved.

#include "Items/UpAmmo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "UpGameInstance.h"
#include "Characters/UpPlayableCharacter.h"
#include "GAS/UpGasDataAsset.h"
#include "GAS/Effects/CustomAppReqs/UpEffectCar_GunHasAmmoReserveSpace.h"
#include "Items/UpWeapon.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool AUpAmmo::HandleAmmoTagSpecGrant(const AUpCharacter* Character, const FUpTagSpec& TagSpec)
{
	if (!TagSpec.RelatedTag.IsValid() || !TagSpec.RelatedTag.MatchesTag(TAG_Item_Weapon)) return false;

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(Character))
	{
		if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
		{
			if (const auto EffectClass = GasDataAsset->GetAmmoGrantEffectClass())
			{
				if (const auto& Equipment = Character->GetCharacterEquipment();
					TryGrantAmmoForWeaponSlot(EffectClass, TagSpec.RelatedTag, TagSpec.Count, Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon1))
					|| TryGrantAmmoForWeaponSlot(EffectClass, TagSpec.RelatedTag, TagSpec.Count, Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon2)))
				{
					return true;
				}
			}
		}
	}

	return false;
}

FUpInteractionData AUpAmmo::GetInteractionData(const AUpPlayerController* PlayerController)
{
	FUpInteractionData InteractionData;
	InteractionData.Interactable = this;
	
	if (const auto& WeaponTag = GetInteractionRelatedTag(PlayerController); WeaponTag.IsValid() && WeaponTag.MatchesTag(TAG_Item_Weapon))
	{
		if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
		{
			if (const auto& WeaponData = GameInstance->GetItemData(WeaponTag); WeaponData.IsValid())
			{
				InteractionData.InteractionPromptText = FText::FromString(
					FString::Printf(TEXT("%s for <RichText.Bold>%s</>"), *ItemData.Name.ToString(), *WeaponData.Name.ToString()));
				
				if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
				{
					if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
					{
						if (const auto EffectClass = GasDataAsset->GetAmmoGrantEffectClass())
						{
							if (const auto& Equipment = PossessedCharacter->GetCharacterEquipment();
								!TryGrantAmmoForWeaponSlot(EffectClass, WeaponTag, 0, Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon1))
								&& !TryGrantAmmoForWeaponSlot(EffectClass, WeaponTag, 0, Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon2)))
							{
								InteractionData.InteractionPromptSubText = FText::FromString(TEXT("(Ammo Full)"));
							}
						}
					}
				}
			}
		}
	}
	
	return InteractionData;
}

int32 AUpAmmo::GetInteractionQuantity(const AUpPlayerController* PlayerController, const FGameplayTag& DynamicRelatedTag) const
{
	// Setting a negative quantity indicates a full magazine.
	if (Quantity < 0.f)
	{
		if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
		{
			if (const auto WeaponData = GameInstance->GetWeaponData(DynamicRelatedTag); WeaponData.IsValid())
			{
				return WeaponData.BaseMagazineCapacity;
			}
		}
	}
	
	return Super::GetInteractionQuantity(PlayerController, DynamicRelatedTag);
}

FGameplayTag AUpAmmo::GetInteractionRelatedTag(const AUpPlayerController* PlayerController) const
{
	// If the ammo's weapon tag isn't explicitly set, set it to the character's active or primary weapon.
	if (!RelatedTag.IsValid())
	{
		if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
		{
			if (const auto& WeaponSlotData = PossessedCharacter->GetEquipment().GetPotentialActiveWeaponSlotData();
				WeaponSlotData.ItemInstance.ItemClass)
			{
				if (const auto DefaultObject = WeaponSlotData.ItemInstance.ItemClass.GetDefaultObject())
				{
					if (const auto& WeaponTag = DefaultObject->GetTagId(); WeaponTag.IsValid())
					{
						return WeaponTag;
					}
				}
			}
		}
	}
	
	return Super::GetInteractionRelatedTag(PlayerController);
}

bool AUpAmmo::TryGrantAmmoForWeaponSlot(const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag& TargetTagId,
	const uint8 Quantity, const FUpEquipmentSlotData& EquipmentSlotData)
{
	if (const auto Weapon = Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor))
	{
		if (!Weapon->GetTagId().MatchesTagExact(TargetTagId)) return false;
	
		if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
		{
			const auto EffectSpecHandle = WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, WeaponAbilitySystemComponent->MakeEffectContext());

			// Quantity <= 0 indicates a test only.
			if (Quantity <= 0.f)
			{
				if (const auto DefaultObject = UUpEffectCar_GunHasAmmoReserveSpace::StaticClass()->GetDefaultObject<UUpEffectCar_GunHasAmmoReserveSpace>())
				{
					const auto EffectSpec = EffectSpecHandle.Data.Get();
					
					return DefaultObject->CanApplyGameplayEffect_Implementation(EffectSpec->Def, *EffectSpec, WeaponAbilitySystemComponent);
				}
			}
			
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, TAG_Item_Ammo, Quantity);

			return WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()).WasSuccessfullyApplied();
		}
	}

	return false;
}
