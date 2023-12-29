// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentVitalStatsDisplayWidget.h"

#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Tags/AttributeTags.h"
#include "UI/UpHud.h"

void UUpPersistentVitalStatsDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->AttributeValueDelegate.AddUObject(this, &ThisClass::HandleAttributeValueChange);
	CustomHud->PossessedCharacterDelegate.AddUObject(this, &ThisClass::HandlePossessedCharacterChange);

	if (const auto Controller = CustomHud->GetCustomController())
	{
		if (const auto Character = Controller->GetPossessedCharacter())
		{
			if (const auto AttributeSet = Character->GetVitalAttributeSet())
			{
				const auto HealthAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_Health);
				const auto MaxHealthAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_MaxHealth);
				const auto ShieldAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_Shield);
				const auto MaxShieldAttribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_MaxShield);

				if (!HealthAttribute.IsValid() || !MaxHealthAttribute.IsValid()) return;

				Health = HealthAttribute.GetNumericValue(AttributeSet);
				MaxHealth = MaxHealthAttribute.GetNumericValue(AttributeSet);
				
				if (!ShieldAttribute.IsValid() || !MaxShieldAttribute.IsValid()) return;

				Shield = ShieldAttribute.GetNumericValue(AttributeSet);
				MaxShield = MaxShieldAttribute.GetNumericValue(AttributeSet);
			}
		}
	}
}

float UUpPersistentVitalStatsDisplayWidget::GetHealthBarPercentage() const
{
	if (MaxHealth <= 0.f) return 0.f;
	
	return Health / MaxHealth;
}

float UUpPersistentVitalStatsDisplayWidget::GetShieldBarPercentage() const
{
	if (MaxShield <= 0.f) return 0.f;
	
	return Shield / MaxShield;
}

void UUpPersistentVitalStatsDisplayWidget::HandleAttributeValueChange(const FGameplayTag& Tag, const float Value)
{
	if (Tag.MatchesTagExact(TAG_Attribute_Vital_Health))
	{
		Health = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Vital_MaxHealth))
	{
		MaxHealth = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Vital_Shield))
	{
		Shield = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Vital_MaxShield))
	{
		MaxShield = Value;
	}
}

void UUpPersistentVitalStatsDisplayWidget::HandlePossessedCharacterChange(const AUpPlayableCharacter* PossessedCharacter)
{
	Image = PossessedCharacter->GetCharacterData().Image_Head;
}
