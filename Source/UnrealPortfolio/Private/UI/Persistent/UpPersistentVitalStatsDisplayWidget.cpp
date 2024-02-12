// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentVitalStatsDisplayWidget.h"

#include "Characters/UpPlayableCharacter.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tags/GasTags.h"
#include "UI/UpHud.h"

void UUpPersistentVitalStatsDisplayWidget::SetCharacter(AUpPlayableCharacter* InCharacter)
{
	const auto bHadCharacter = Character != nullptr;
	
	Character = InCharacter;
	Image = InCharacter->GetCharacterData().Image_Head;

	UpdateValues();

	if (bHadCharacter)
	{
		Health = TargetHealth;
		Shield = TargetShield;
		Stamina = TargetStamina;
	}
}

void UUpPersistentVitalStatsDisplayWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	UpdateValues();

	Health = UKismetMathLibrary::FInterpTo(Health, TargetHealth, InDeltaTime, InterpSpeed);
	Shield = UKismetMathLibrary::FInterpTo(Shield, TargetShield, InDeltaTime, InterpSpeed);
	Stamina = UKismetMathLibrary::FInterpTo(Stamina, TargetStamina, InDeltaTime, InterpSpeed);
}

void UUpPersistentVitalStatsDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	// CustomHud->AttributeValueDelegate.AddUObject(this, &ThisClass::HandleAttributeValueChange);
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

float UUpPersistentVitalStatsDisplayWidget::GetStaminaBarPercentage() const
{
	if (MaxStamina <= 0.f) return 0.f;
	
	return Stamina / MaxStamina;
}

ESlateVisibility UUpPersistentVitalStatsDisplayWidget::HideIfSecondary() const
{
	if (bSecondary) return ESlateVisibility::Collapsed;

	return ESlateVisibility::SelfHitTestInvisible;
}

ESlateVisibility UUpPersistentVitalStatsDisplayWidget::ShowIfSecondary() const
{
	if (!bSecondary) return ESlateVisibility::Collapsed;

	return ESlateVisibility::SelfHitTestInvisible;
}

FLinearColor UUpPersistentVitalStatsDisplayWidget::GetImageOpacity() const
{
	if (!Character || Character->IsDead()) return FLinearColor(1.f, 1.f, 1.f, 0.6f);

	return FLinearColor(1.f, 1.f, 1.f, 1.f);
}

ESlateVisibility UUpPersistentVitalStatsDisplayWidget::GetCommonActionWidgetVisibility() const
{
	if (!Character || Character->IsDead()) return ESlateVisibility::Collapsed;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentVitalStatsDisplayWidget::HandleAttributeValueChange(const FGameplayTag& TagId, const FGameplayTag& AttributeTag, const float Value)
{
	if (!Character || !TagId.MatchesTagExact(Character->GetTagId())) return;
	
	if (AttributeTag.MatchesTagExact(TAG_Attribute_Vital_Health))
	{
		TargetHealth = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Vital_MaxHealth))
	{
		MaxHealth = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Vital_Shield))
	{
		TargetShield = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Vital_MaxShield))
	{
		MaxShield = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Vital_Stamina))
	{
		TargetStamina = Value;
	} else if (AttributeTag.MatchesTagExact(TAG_Attribute_Vital_MaxStamina))
	{
		MaxStamina = Value;
	}
}

void UUpPersistentVitalStatsDisplayWidget::UpdateValues()
{
	if (!Character) return;
	
	if (const auto AttributeSet = Character->GetVitalAttributeSet())
	{
		TargetHealth = AttributeSet->GetHealth();
		MaxHealth = AttributeSet->GetMaxHealth();
		TargetShield = AttributeSet->GetShield();
		MaxShield = AttributeSet->GetMaxShield();
		TargetStamina = AttributeSet->GetStamina();
		MaxStamina = AttributeSet->GetMaxStamina();
	}
}
