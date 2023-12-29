// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentAmmoDisplayWidget.h"

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

	if (const auto Controller = CustomHud->GetCustomController())
	{
		if (const auto Character = Controller->GetPossessedCharacter())
		{
			if (const auto AttributeSet = Character->GetVitalAttributeSet())
			{
				// const auto HealthAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_Health);
				// const auto MaxHealthAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_MaxHealth);
				// const auto ShieldAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_Shield);
				// const auto MaxShieldAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_MaxShield);
				//
				// if (!HealthAttribute.IsValid() || !MaxHealthAttribute.IsValid()) return;
				//
				// Health = HealthAttribute.GetNumericValue(AttributeSet);
				// MaxHealth = MaxHealthAttribute.GetNumericValue(AttributeSet);
				//
				// if (!ShieldAttribute.IsValid() || !MaxShieldAttribute.IsValid()) return;
				//
				// Shield = ShieldAttribute.GetNumericValue(AttributeSet);
				// MaxShield = MaxShieldAttribute.GetNumericValue(AttributeSet);
			}
		}
	}
}

void UUpPersistentAmmoDisplayWidget::HandleActiveWeaponChange(const AUpWeapon* Weapon)
{
	ItemData = Weapon ? Weapon->GetItemData() : FUpItemData();
}

void UUpPersistentAmmoDisplayWidget::HandleAttributeValueChange(const FGameplayTag& Tag, const float Value)
{
	if (Tag.MatchesTagExact(TAG_Attribute_Ammo_AmmoReserve))
	{
		UE_LOG(LogTemp, Warning, TEXT("attribute value change: %s - %g"), *Tag.ToString(), Value)
	}
}
