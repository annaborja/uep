// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/StatsMenu/UpStatsMenuWidget.h"

#include "GameplayTagContainer.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "UI/Basic/UpAttributeBarWidget.h"
#include "UI/UpHud.h"

void UUpStatsMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulatePrimaryAttributes();
}

void UUpStatsMenuWidget::PopulatePrimaryAttributes()
{
	if (!CustomHud) return;
	
	if (const auto CustomController = CustomHud->GetCustomController())
	{
		if (const auto PlayerState = CustomController->GetPlayerState<AUpPlayerState>())
		{
			if (const auto PrimaryAttributeSet = PlayerState->GetPrimaryAttributeSet())
			{
				if (const auto PrimaryAttributesContainer = GetPrimaryAttributesContainer())
				{
					PrimaryAttributesContainer->ClearChildren();
					
					uint8 WidgetIndex = 0;
		
					for (const auto& Tag : PrimaryAttributeTags)
					{
						const auto Widget = CreateWidget<UUpAttributeBarWidget>(this, PrimaryAttributeDisplayWidgetClass);
						Widget->SetCustomHud(CustomHud);
						Widget->PopulateAttribute(Tag, PrimaryAttributeSet);

						if (const auto Slot = Cast<UVerticalBoxSlot>(PrimaryAttributesContainer->AddChild(Widget)); Slot && WidgetIndex > 0)
						{
							Slot->SetPadding(FMargin(0.f, PrimaryAttributeRowGap, 0.f, 0.f));
						}

						WidgetIndex++;
					}
				}
			}
		}
	}
}
