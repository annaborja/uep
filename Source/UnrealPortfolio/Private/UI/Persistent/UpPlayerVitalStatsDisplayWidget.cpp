// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPlayerVitalStatsDisplayWidget.h"

#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
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
		if (const auto PlayerState = Controller->GetPlayerState<AUpPlayerState>())
		{
			if (const auto AttributeSet = PlayerState->GetHealthAttributeSet())
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
	UE_LOG(LogTemp, Warning, TEXT("handle att change: %s"), *Tag.ToString())
	if (Tag.MatchesTagExact(TAG_Attribute_Health_Health))
	{
		Health = Value;
	} else if (Tag.MatchesTagExact(TAG_Attribute_Health_MaxHealth))
	{
		MaxHealth = Value;
	}
}
