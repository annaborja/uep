// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/UpMainMenuTabListWidget.h"

#include "CommonButtonBase.h"
#include "Components/PanelWidget.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/UpMainMenuTabWidget.h"

void UUpMainMenuTabListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	OnTabButtonCreation.AddDynamic(this, &ThisClass::AddTabButton);
	OnTabButtonRemoval.AddDynamic(this, &ThisClass::RemoveTabButton);
}

void UUpMainMenuTabListWidget::AddTabButton(FName TabId, UCommonButtonBase* TabButton)
{
	if (const auto MainMenuTab = Cast<UUpMainMenuTabWidget>(TabButton); MainMenuTab && MenuTabDataTable)
	{
		TArray<FUpMenuTabData*> AllMenuTabDataRows;
		MenuTabDataTable->GetAllRows(TEXT("MenuTabData GetAllRows"), AllMenuTabDataRows);

		FUpMenuTabData MenuTabData;

		for (const auto Row : AllMenuTabDataRows)
		{
			if (Row->TagId.GetTagName() == TabId)
			{
				MenuTabData = *Row;
				break;
			}
		}

		if (MenuTabData.IsValid())
		{
			MainMenuTab->SetLabel(MenuTabData.Label);

			if (const auto TabsContainer = GetTabsContainer())
			{
				TabsContainer->AddChild(MainMenuTab);
			}
		}
	}
}

void UUpMainMenuTabListWidget::RemoveTabButton(FName TabId, UCommonButtonBase* TabButton)
{
	if (const auto TabsContainer = GetTabsContainer())
	{
		TabsContainer->RemoveChild(TabButton);
	}
}
