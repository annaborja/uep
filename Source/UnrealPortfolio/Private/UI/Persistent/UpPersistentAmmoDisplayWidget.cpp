// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentAmmoDisplayWidget.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpWeapon.h"
#include "Tags/GasTags.h"
#include "UI/UpHud.h"

ESlateVisibility UUpPersistentAmmoDisplayWidget::GetRootVisibility() const
{
	if (!ItemData.IsValid()) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentAmmoDisplayWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateValues();
}

void UUpPersistentAmmoDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->ActiveWeaponDelegate.AddUObject(this, &ThisClass::HandleActiveWeaponChange);
	// CustomHud->AttributeValueDelegate.AddUObject(this, &ThisClass::HandleAttributeValueChange);

	if (const auto CustomController = CustomHud->GetCustomController())
	{
		if (const auto PossessedCharacter = CustomController->GetPossessedCharacter())
		{
			if (const auto EquipmentSlotData = PossessedCharacter->GetEquipment().GetPotentialActiveWeaponSlotData(); EquipmentSlotData.bActivated)
			{
				Weapon = Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor);
				UpdateValues();
			}
		}
	}
}

void UUpPersistentAmmoDisplayWidget::HandleActiveWeaponChange(AUpWeapon* InWeapon)
{
	Weapon = InWeapon;
	UpdateValues();
}

void UUpPersistentAmmoDisplayWidget::HandleAttributeValueChange(const FGameplayTag& TagId, const FGameplayTag& AttributeTag, const float Value)
{
	if (AttributeTag.MatchesTagExact(TAG_Attribute_Ammo_MagazineFill))
	{
		MagazineFill = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Ammo_MagazineCapacity))
	{
		MagazineCapacity = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Ammo_AmmoReserve))
	{
		AmmoReserve = Value;
	}
}

void UUpPersistentAmmoDisplayWidget::UpdateValues()
{
	if (Weapon)
	{
		ItemData = Weapon->GetItemData();

		if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
		{
			MagazineFill = AttributeSet->GetMagazineFill();
			MagazineCapacity = AttributeSet->GetMagazineCapacity();
			AmmoReserve = AttributeSet->GetAmmoReserve();
		}
	} else
	{
		ItemData = FUpItemData();
		MagazineFill = 0.f;
		MagazineCapacity = 0.f;
		AmmoReserve = 0.f;
	}
}
