// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/ReputationMenu/UpReputationMenuWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/ReputationMenu/UpPlayerNpcReputationDataWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpReputationMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (CustomHud)
	{
		PlayerWorldReputationData = CustomHud->GetPlayerWorldReputationData();
		PlayerNpcReputationDataMap = CustomHud->GetPlayerNpcReputationDataMap();
	}
	
	PopulateNpcReputationData();
}

void UUpReputationMenuWidget::PopulateNpcReputationData()
{
	if (const auto NpcReputationDataContainer = GetNpcReputationDataContainer())
	{
		NpcReputationDataContainer->ClearChildren();

		TArray<UUpPlayerNpcReputationDataWidget*> ReputationDataWidgets;

		for (const auto& ReputationDataMapping : PlayerNpcReputationDataMap)
		{
			const auto ReputationDataWidget = CreateWidget<UUpPlayerNpcReputationDataWidget>(this, NpcReputationDataWidgetClass);
			ReputationDataWidget->SetNpcName(UUpBlueprintFunctionLibrary::GetInGameName(this, ReputationDataMapping.Key));
			ReputationDataWidget->SetReputationData(ReputationDataMapping.Value);
			
			ReputationDataWidgets.Add(ReputationDataWidget);
		}

		// TODO(P0): Sorting isn't taking effect in the array for some reason.
		ReputationDataWidgets.Sort([](const UUpPlayerNpcReputationDataWidget& WidgetA, const UUpPlayerNpcReputationDataWidget& WidgetB)
		{
			return WidgetA.GetNpcName().ToString().Compare(WidgetB.GetNpcName().ToString());
		});
		
		uint8 ReputationDataWidgetIndex = 0;

		for (const auto ReputationDataWidget : ReputationDataWidgets)
		{
			if (const auto PanelSlot = Cast<UVerticalBoxSlot>(NpcReputationDataContainer->AddChild(ReputationDataWidget));
					PanelSlot && ReputationDataWidgetIndex > 0)
			{
				PanelSlot->SetPadding(FMargin(0.0, ReputationDataRowGap, 0.0, 0.0));
			}
				
			ReputationDataWidgetIndex++;
		}
	}
}
