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

	if (const auto CustomController = CustomHud->GetCustomController())
	{
		if (const auto PossessedCharacter = CustomController->GetPossessedCharacter())
		{
			InitAttributes(PossessedCharacter);
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

	InitAttributes(PossessedCharacter);
}

void UUpPersistentVitalStatsDisplayWidget::InitAttributes(const AUpPlayableCharacter* PossessedCharacter)
{
	if (const auto AttributeSet = PossessedCharacter->GetVitalAttributeSet())
	{
		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_Health); Attribute.IsValid())
		{
			Health = Attribute.GetNumericValue(AttributeSet);
		}

		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_MaxHealth); Attribute.IsValid())
		{
			MaxHealth = Attribute.GetNumericValue(AttributeSet);
		}
		
		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_Shield); Attribute.IsValid())
		{
			Shield = Attribute.GetNumericValue(AttributeSet);
		}

		if (const auto Attribute = AttributeSet->GetAttribute(TAG_Attribute_Vital_MaxShield); Attribute.IsValid())
		{
			MaxShield = Attribute.GetNumericValue(AttributeSet);
		}
	}
}
