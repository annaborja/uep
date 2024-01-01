// Copyright AB. All Rights Reserved.

#include "Components/UpInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "UpGameInstance.h"
#include "Characters/UpCharacter.h"
#include "GAS/UpGasDataAsset.h"
#include "Items/UpItem.h"
#include "Items/UpWeapon.h"
#include "Utils/Structs.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpInventoryComponent::UUpInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UUpInventoryComponent::HandleTagSpecGrant(const FUpTagSpec& TagSpec) const
{
	if (TagSpec.Tag.MatchesTagExact(TAG_Item_Ammo)) return HandleAmmoTagSpecGrant(TagSpec);
	
	return false;
}

bool UUpInventoryComponent::HandleAmmoTagSpecGrant(const FUpTagSpec& TagSpec) const
{
	if (!TagSpec.RelatedTag.IsValid() || !IsWeaponTag(TagSpec.RelatedTag)) return false;

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
		{
			if (const auto AmmoGrantEffectClass = GasDataAsset->GetAmmoGrantEffectClass())
			{
				if (const auto Character = Cast<AUpCharacter>(GetOwner()))
				{
					const auto& Equipment = Character->GetCharacterEquipment();

					if (TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagSpec.RelatedTag, TagSpec.Count,
						Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon1)))
					{
						return true;
					}
					
					if (TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagSpec.RelatedTag, TagSpec.Count,
						Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon2)))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool UUpInventoryComponent::TryGrantAmmoForWeaponSlot(const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag& TargetTagId,
	const uint8 Quantity, const FUpEquipmentSlotData& EquipmentSlotData) const
{
	if (const auto Weapon = Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor))
	{
		if (!Weapon->GetTagId().MatchesTagExact(TargetTagId)) return false;
	
		if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
		{
			const auto EffectSpecHandle = WeaponAbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, WeaponAbilitySystemComponent->MakeEffectContext());
					
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, TAG_Item_Ammo, Quantity);

			return WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()).WasSuccessfullyApplied();
		}
	}

	return false;
}
