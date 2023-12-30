// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentAmmoDisplayWidget.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"
#include "Tags/AttributeTags.h"
#include "UI/UpHud.h"

ESlateVisibility UUpPersistentAmmoDisplayWidget::GetRootVisibility() const
{
	if (!ItemData.IsValid()) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentAmmoDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->ActiveWeaponDelegate.AddUObject(this, &ThisClass::HandleActiveWeaponChange);
	CustomHud->AttributeValueDelegate.AddUObject(this, &ThisClass::HandleAttributeValueChange);

	if (const auto CustomController = CustomHud->GetCustomController())
	{
		if (const auto PossessedCharacter = CustomController->GetPossessedCharacter())
		{
			if (const auto EquipmentSlotData = PossessedCharacter->GetEquipment().GetPotentialActiveWeaponSlotData(); EquipmentSlotData.bActivated)
			{
				if (const auto Weapon = Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor))
				{
					InitAttributes(Weapon);
				}
			}
		}
	}
}

void UUpPersistentAmmoDisplayWidget::HandleActiveWeaponChange(const AUpWeapon* Weapon)
{
	if (Weapon)
	{
		ItemData = Weapon->GetItemData();

		InitAttributes(Weapon);
	} else
	{
		ItemData = FUpItemData();
		MagazineFill = 0.f;
		MagazineCapacity = 0.f;
		AmmoReserve = 0.f;
	}
}

void UUpPersistentAmmoDisplayWidget::HandleAttributeValueChange(const FGameplayTag& Tag, const float Value)
{
	if (Tag.MatchesTagExact(TAG_Attribute_Ammo_MagazineFill))
	{
		MagazineFill = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Ammo_MagazineCapacity))
	{
		MagazineCapacity = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Ammo_AmmoReserve))
	{
		AmmoReserve = Value;
	}
}

void UUpPersistentAmmoDisplayWidget::InitAttributes(const AUpWeapon* Weapon)
{
	if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
	{
		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Ammo_MagazineFill); Attribute.IsValid())
		{
			MagazineFill = Attribute.GetNumericValue(AttributeSet);
		}
		
		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Ammo_MagazineCapacity); Attribute.IsValid())
		{
			MagazineCapacity = Attribute.GetNumericValue(AttributeSet);
		}
		
		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Ammo_AmmoReserve); Attribute.IsValid())
		{
			AmmoReserve = Attribute.GetNumericValue(AttributeSet);
		}
	}
}
