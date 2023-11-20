// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/ReputationMenu/UpPlayerNpcReputationDataWidget.h"
#include "UI/MainMenu/ReputationMenu/UpReputationAttributeWidget.h"

#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"

void UUpPlayerNpcReputationDataWidget::PopulateReputationData(const FUpReputationData& ReputationData)
{
	if (const auto ReputationAttributesContainer = GetReputationAttributesContainer(); ReputationAttributesContainer && ReputationAttributeWidgetClass)
	{
		const auto AffectionWidget = CreateWidget<UUpReputationAttributeWidget>(this, ReputationAttributeWidgetClass);
		AffectionWidget->SetLabel(FText::FromString(TEXT("Affection")));
		AffectionWidget->PopulateValue(ReputationData.Affection,
			UUpPlayerReputationComponent::AffectionMinValue, UUpPlayerReputationComponent::AffectionMaxValue);
		
		const auto EsteemWidget = CreateWidget<UUpReputationAttributeWidget>(this, ReputationAttributeWidgetClass);
		EsteemWidget->SetLabel(FText::FromString(TEXT("Esteem")));
		AffectionWidget->PopulateValue(ReputationData.Esteem,
			UUpPlayerReputationComponent::EsteemMinValue, UUpPlayerReputationComponent::EsteemMaxValue);
			
		ReputationAttributesContainer->AddChild(AffectionWidget);
		
		if (const auto PanelSlot = Cast<UVerticalBoxSlot>(ReputationAttributesContainer->AddChild(EsteemWidget)))
		{
			PanelSlot->SetPadding(FMargin(0.0, ReputationAttributeRowGap, 0.0, 0.0));
		}
	}
}
