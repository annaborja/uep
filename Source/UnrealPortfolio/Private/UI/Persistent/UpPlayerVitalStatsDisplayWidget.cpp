// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPlayerVitalStatsDisplayWidget.h"

#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Tags/AttributeTags.h"
#include "UI/UpHud.h"

void UUpPlayerVitalStatsDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->AttributeValueDelegate.AddDynamic(this, &ThisClass::HandleAttributeValueChange);

	if (const auto Controller = CustomHud->GetCustomController())
	{
		if (const auto PlayerState = Controller->GetPlayerState<AUpPlayerState>())
		{
			if (const auto VitalAttributeSet = PlayerState->GetVitalAttributeSet())
			{
				const auto HealthAttribute = VitalAttributeSet->GetAttribute(TAG_Attribute_Vital_Health);
				const auto MaxHealthAttribute = VitalAttributeSet->GetAttribute(TAG_Attribute_Vital_MaxHealth);

				if (!HealthAttribute.IsValid() || !MaxHealthAttribute.IsValid()) return;

				Health = HealthAttribute.GetNumericValue(VitalAttributeSet);
				MaxHealth = MaxHealthAttribute.GetNumericValue(VitalAttributeSet);
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
	if (Tag.MatchesTagExact(TAG_Attribute_Vital_Health))
	{
		Health = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Vital_MaxHealth))
	{
		MaxHealth = Value;
	}
}
