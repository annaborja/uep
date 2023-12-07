// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/ReputationMenu/UpPlayerNpcReputationDisplayWidget.h"
#include "UI/MainMenu/ReputationMenu/UpReputationAttributeDisplayWidget.h"

#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"

void UUpPlayerNpcReputationDisplayWidget::PopulateReputationData(const FUpReputationData& ReputationData)
{
	if (const auto ReputationAttributesContainer = GetReputationAttributesContainer(); ReputationAttributesContainer && ReputationAttributeDisplayWidgetClass)
	{
		const auto AffectionWidget = CreateWidget<UUpReputationAttributeDisplayWidget>(this, ReputationAttributeDisplayWidgetClass);
		AffectionWidget->SetLabel(AffectionLabel);
		AffectionWidget->PopulateValue(ReputationData.Affection,
			UUpPlayerReputationComponent::AffectionMinValue, UUpPlayerReputationComponent::AffectionMaxValue);
		
		const auto EsteemWidget = CreateWidget<UUpReputationAttributeDisplayWidget>(this, ReputationAttributeDisplayWidgetClass);
		EsteemWidget->SetLabel(EsteemLabel);
		EsteemWidget->PopulateValue(ReputationData.Esteem,
			UUpPlayerReputationComponent::EsteemMinValue, UUpPlayerReputationComponent::EsteemMaxValue);
			
		ReputationAttributesContainer->AddChild(AffectionWidget);
		
		if (const auto PanelSlot = Cast<UVerticalBoxSlot>(ReputationAttributesContainer->AddChild(EsteemWidget)))
		{
			PanelSlot->SetPadding(FMargin(0.f, ReputationAttributeRowGap, 0.f, 0.f));
		}
	}
}
