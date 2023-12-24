// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPlayerVitalStatsDisplayWidget.h"

#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "GAS/Attributes/UpHealthAttributeSet.h"
#include "Tags/AttributeTags.h"
#include "UI/UpHud.h"

void UUpPlayerVitalStatsDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->AttributeValueDelegate.AddDynamic(this, &ThisClass::HandleAttributeValueChange);

	if (const auto Controller = CustomHud->GetCustomController())
	{
		if (const auto Character = Controller->GetPossessedCharacter())
		{
			if (const auto AttributeSet = Character->GetHealthAttributeSet())
			{
				const auto HealthAttribute = AttributeSet->GetAttribute(TAG_Attribute_Health_Health);
				const auto MaxHealthAttribute = AttributeSet->GetAttribute(TAG_Attribute_Health_MaxHealth);

				if (!HealthAttribute.IsValid() || !MaxHealthAttribute.IsValid()) return;

				Health = HealthAttribute.GetNumericValue(AttributeSet);
				MaxHealth = MaxHealthAttribute.GetNumericValue(AttributeSet);
			}
		}
	}
}

float UUpPlayerVitalStatsDisplayWidget::GetHealthBarPercentage() const
{
	if (MaxHealth <= 0.f) return 0.f;
	
	return Health / MaxHealth;
}

void UUpPlayerVitalStatsDisplayWidget::HandleAttributeValueChange(const FGameplayTag& Tag, const float Value)
{
	if (Tag.MatchesTagExact(TAG_Attribute_Health_Health))
	{
		Health = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Health_MaxHealth))
	{
		MaxHealth = Value;
	}
}
