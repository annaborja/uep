// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/ReputationMenu/UpReputationMenuWidget.h"

#include "UpGameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/MainMenu/ReputationMenu/UpPlayerNpcReputationDisplayWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpReputationMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		PlayerWorldReputationData = GameInstance->GetPlayerWorldReputationData();
		PlayerNpcReputationDataMap = GameInstance->GetPlayerNpcReputationDataMap();
	}
	
	PopulateNpcReputations();
}

void UUpReputationMenuWidget::PopulateNpcReputations()
{
	if (const auto NpcReputationsContainer = GetNpcReputationsContainer())
	{
		NpcReputationsContainer->ClearChildren();

		TArray<UUpPlayerNpcReputationDisplayWidget*> Widgets;

		for (const auto& Mapping : PlayerNpcReputationDataMap)
		{
			const auto Widget = CreateWidget<UUpPlayerNpcReputationDisplayWidget>(this, NpcReputationDisplayWidgetClass);
			Widget->SetNpcName(UUpBlueprintFunctionLibrary::GetInGameName(this, Mapping.Key));
			Widget->PopulateReputationData(Mapping.Value);
			Widgets.Add(Widget);
		}

		// TODO(P0): Sorting isn't taking effect in the array for some reason.
		Widgets.Sort([](const UUpPlayerNpcReputationDisplayWidget& WidgetA, const UUpPlayerNpcReputationDisplayWidget& WidgetB)
		{
			return WidgetA.GetNpcName().ToString().Compare(WidgetB.GetNpcName().ToString());
		});
		
		uint8 WidgetIndex = 0;

		for (const auto Widget : Widgets)
		{
			if (const auto Slot = Cast<UVerticalBoxSlot>(NpcReputationsContainer->AddChild(Widget)); Slot && WidgetIndex > 0)
			{
				Slot->SetPadding(FMargin(0.f, NpcReputationRowGap, 0.f, 0.f));
			}
				
			WidgetIndex++;
		}
	}
}
