// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMemberStatsMenuWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "UI/Basic/UpAttributeBarWidget.h"
#include "UI/Basic/UpProgressBarWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMemberStatsMenuWidget::SetNpc(const AUpPlayableNpc* Npc)
{
	bHasFullData = true;
	
	PopulatePrimaryAttributes(Npc);
	PopulatePlayerSentiment(Npc->GetTagId());
}

void UUpSquadMemberStatsMenuWidget::SetNpcTagId(const FGameplayTag& NpcTagId)
{
	bHasFullData = false;
	
	PopulatePlayerSentiment(NpcTagId);
}

void UUpSquadMemberStatsMenuWidget::PopulatePrimaryAttributes(const AUpPlayableNpc* Npc)
{
	if (const auto PrimaryAttributesContainer = GetPrimaryAttributesContainer())
	{
		PrimaryAttributesContainer->ClearChildren();

		if (const auto PrimaryAttributeSet = Npc->GetPrimaryAttributeSet())
		{
			uint8 PrimaryAttributeIndex = 0;

			for (const auto& Tag : PrimaryAttributeTags)
			{
				const auto Widget = CreateWidget<UUpAttributeBarWidget>(this, PrimaryAttributeDisplayWidgetClass);
				Widget->SetCustomHud(CustomHud);
				Widget->PopulateAttribute(Tag, PrimaryAttributeSet);

				if (const auto Slot = Cast<UVerticalBoxSlot>(PrimaryAttributesContainer->AddChild(Widget)); Slot && PrimaryAttributeIndex > 0)
				{
					Slot->SetPadding(FMargin(0.f, PrimaryAttributeRowGap, 0.f, 0.f));
				}

				PrimaryAttributeIndex++;
			}
		}
	}
}

void UUpSquadMemberStatsMenuWidget::PopulatePlayerSentiment(const FGameplayTag& NpcTagId)
{
	FUpReputationData PlayerReputationData;
			
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		PlayerReputationData = GameInstance->GetPlayerReputationData(NpcTagId);
	}

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
