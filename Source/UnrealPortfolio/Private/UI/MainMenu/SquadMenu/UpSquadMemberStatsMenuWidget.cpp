// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberStatsMenuWidget.h"

#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/Basic/UpProgressBarWidget.h"

void UUpSquadMemberStatsMenuWidget::PopulatePlayerSentiment(const FUpReputationData& PlayerReputationData)
{
	if (const auto Container = GetPlayerSentimentAttributesContainer(); Container && SentimentAttributeDisplayWidgetClass)
	{
		const auto AffectionWidget = CreateWidget<UUpProgressBarWidget>(this, SentimentAttributeDisplayWidgetClass);
		AffectionWidget->SetLabel(AffectionLabel);
		AffectionWidget->PopulateValue(PlayerReputationData.Affection,
			UUpPlayerReputationComponent::AffectionMinValue, UUpPlayerReputationComponent::AffectionMaxValue);
		
		const auto EsteemWidget = CreateWidget<UUpProgressBarWidget>(this, SentimentAttributeDisplayWidgetClass);
		EsteemWidget->SetLabel(EsteemLabel);
		EsteemWidget->PopulateValue(PlayerReputationData.Esteem,
			UUpPlayerReputationComponent::EsteemMinValue, UUpPlayerReputationComponent::EsteemMaxValue);
			
		Container->AddChild(AffectionWidget);
		
		if (const auto PanelSlot = Cast<UVerticalBoxSlot>(Container->AddChild(EsteemWidget)))
		{
			PanelSlot->SetPadding(FMargin(0.f, SentimentAttributeRowGap, 0.f, 0.f));
		}
	}
}

void UUpSquadMemberStatsMenuWidget::SetAbilitySystemComponent(UUpAbilitySystemComponent* InAbilitySystemComponent)
{
}

void UUpSquadMemberStatsMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulatePrimaryAttributes();
}

void UUpSquadMemberStatsMenuWidget::PopulatePrimaryAttributes()
{
}
