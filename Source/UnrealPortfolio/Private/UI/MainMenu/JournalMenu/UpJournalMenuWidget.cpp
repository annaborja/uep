// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/JournalMenu/UpJournalMenuWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/JournalMenu/UpCodexMenuWidget.h"
#include "UI/MainMenu/JournalMenu/UpObjectivesMenuWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberSubMenuNavButtonWidget.h"

void UUpJournalMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	PopulateSubMenuNav();
}

void UUpJournalMenuWidget::SetActiveSubMenu(const FGameplayTag& SubMenuTag) const
{
	if (const auto SubMenuSwitcher = GetSubMenuSwitcher())
	{
		UWidget* Widget;

		if (SubMenuTag.MatchesTagExact(TAG_Menu_Journal_Codex))
		{
			Widget = GetCodexMenu();
		} else
		{
			Widget = GetObjectivesMenu();
		}

		if (Widget) SubMenuSwitcher->SetActiveWidget(Widget);
	}
}

void UUpJournalMenuWidget::ShowNextSubMenu()
{
	if (CurrentSubMenuIndex >= SubMenuTags.Num() - 1)
	{
		CurrentSubMenuIndex = 0;
	} else
	{
		CurrentSubMenuIndex++;
	}
	
	SelectSubMenu();
}

void UUpJournalMenuWidget::ShowPreviousSubMenu()
{
	if (CurrentSubMenuIndex <= 0)
	{
		CurrentSubMenuIndex = SubMenuTags.Num() - 1;
	} else
	{
		CurrentSubMenuIndex--;
	}
	
	SelectSubMenu();
}

void UUpJournalMenuWidget::PopulateSubMenuNav()
{
	TArray<FUpMenuTabData*> AllMenuTabDataRows;
	if (MenuTabDataTable) MenuTabDataTable->GetAllRows(TEXT("MenuTabDataTable GetAllRows"), AllMenuTabDataRows);

	if (const auto Nav = GetSubMenuNav(); Nav && SubMenuNavButtonClass)
	{
		Nav->ClearChildren();

		uint8 WidgetIndex = 0;
		
		for (const auto& SubMenuTag : SubMenuTags)
		{
			FUpMenuTabData MenuTabData;

			for (const auto Row : AllMenuTabDataRows)
			{
				if (Row->TagId.MatchesTagExact(SubMenuTag))
				{
					MenuTabData = *Row;
					break;
				}
			}
			
			const auto Widget = CreateWidget<UUpSquadMemberSubMenuNavButtonWidget>(this, SubMenuNavButtonClass);
			Widget->SetCustomHud(CustomHud);
			Widget->SetLabel(MenuTabData.IsValid() ? MenuTabData.Label : FText::FromString(SubMenuTag.ToString()));
			
			Widget->OnClicked().AddLambda([this, Nav, &SubMenuTag]
			{
				for (const auto ChildWidget : Nav->GetAllChildren())
				{
					if (const auto Button = Cast<UUpSquadMemberSubMenuNavButtonWidget>(ChildWidget))
					{
						// The base UE button widget will handle toggling the clicked button as selected,
						// so we don't need to do that ourselves.
						Button->SetIsSelected(false);
					}
				}

				SetActiveSubMenu(SubMenuTag);
			});
			
			Nav->AddChild(Widget);

			if (WidgetIndex <= 0) Widget->SetIsSelected(true);
			
			// Set the sound override after the first widget is activated
			// so that the sound won't play upon initialization.
			Widget->SetPressedSoundOverride(Sfx_SubMenuSwitch);
			
			WidgetIndex++;
		}
	}
}

void UUpJournalMenuWidget::SelectSubMenu()
{
	if (!SubMenuTags.IsValidIndex(CurrentSubMenuIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectSubMenu: Invalid sub-menu index %d"), CurrentSubMenuIndex)
		return;
	}

	if (const auto Nav = GetSubMenuNav())
	{
		uint8 WidgetIndex = 0;
		
		for (const auto ChildWidget : Nav->GetAllChildren())
		{
			if (const auto Button = Cast<UUpBasicButtonWidget>(ChildWidget))
			{
				Button->SetIsSelected(WidgetIndex == CurrentSubMenuIndex ? true : false);
			}

			WidgetIndex++;
		}
	}

	SetActiveSubMenu(SubMenuTags[CurrentSubMenuIndex]);
}
