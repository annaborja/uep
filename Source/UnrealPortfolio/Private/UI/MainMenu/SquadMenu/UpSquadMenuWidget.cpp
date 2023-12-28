// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMenuWidget.h"

#include "UpGameInstance.h"
#include "CommonWidgetCarousel.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpHud.h"
#include "UI/Basic/UpBasicButtonWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberDisplayWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberNavButtonWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberSubMenuNavButtonWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulateSquadMembers();
	PopulateSubMenuNav();
}

void UUpSquadMenuWidget::ShowNextSquadMember()
{
	if (CurrentSquadMemberIndex >= SquadMemberTags.Num() - 1)
	{
		CurrentSquadMemberIndex = 0;
	} else
	{
		CurrentSquadMemberIndex++;
	}
	
	SelectSquadMember();
}

void UUpSquadMenuWidget::ShowPreviousSquadMember()
{
	if (CurrentSquadMemberIndex <= 0)
	{
		CurrentSquadMemberIndex = SquadMemberTags.Num() - 1;
	} else
	{
		CurrentSquadMemberIndex--;
	}
	
	SelectSquadMember();
}

void UUpSquadMenuWidget::ShowNextSubMenu()
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

void UUpSquadMenuWidget::ShowPreviousSubMenu()
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

void UUpSquadMenuWidget::PopulateSquadMembers()
{
	SquadMemberTags.Reset();
	
	const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
	
	if (GameInstance)
	{
		FGameplayTagContainer PartyMemberTags;
		GameInstance->GetPartyMemberTags(PartyMemberTags);
		PartyMemberTags.GetGameplayTagArray(SquadMemberTags);

		// TODO(P0): Sorting doesn't seem to be working for some reason.
		SquadMemberTags.Sort([this] (const FGameplayTag& TagA, const FGameplayTag& TagB)
		{
			return UUpBlueprintFunctionLibrary::GetInGameName(this, TagA).ToString().Compare(
				UUpBlueprintFunctionLibrary::GetInGameName(this, TagB).ToString());
		});
	}

	if (const auto Carousel = GetSquadMembersCarousel())
	{
		Carousel->ClearChildren();
		SubMenuNavigationDelegate.Clear();

		const auto Nav = GetSquadMembersNav();
		uint8 SquadMemberIndex = 0;

		if (Nav) Nav->ClearChildren();
		
		TArray<AActor*> NpcActors;
		UGameplayStatics::GetAllActorsOfClass(this, AUpPlayableNpc::StaticClass(), NpcActors);
		
		for (const auto& SquadMemberTag : SquadMemberTags)
		{
			AUpPlayableNpc* Npc = nullptr;
			FUpCharacterData CharacterData;

			for (const auto Actor : NpcActors)
			{
				if (UUpBlueprintFunctionLibrary::HasTagId(Actor, SquadMemberTag))
				{
					Npc = Cast<AUpPlayableNpc>(Actor);
					break;
				}
			}
			
			if (Npc)
			{
				CharacterData = Npc->GetCharacterData();
			} else if (GameInstance)
			{
				CharacterData = GameInstance->GetCharacterData(SquadMemberTag);
			}
			
			if (SquadMemberDisplayWidgetClass)
			{
				auto Widget = CreateWidget<UUpSquadMemberDisplayWidget>(this, SquadMemberDisplayWidgetClass);

				if (Npc)
				{
					Widget->SetNpc(Npc);
				} else
				{
					Widget->SetCharacterData(CharacterData);
				}
				
				SubMenuNavigationDelegate.AddLambda([Widget](const FGameplayTag& SubMenuTag)
				{
					Widget->SetActiveSubMenu(SubMenuTag);
				});
				
				Carousel->AddChild(Widget);
			}

			if (SquadMemberNavButtonClass && Nav)
			{
				const auto Widget = CreateWidget<UUpSquadMemberNavButtonWidget>(this, SquadMemberNavButtonClass);

				if (CharacterData.IsValid()) Widget->SetImage(CharacterData.Image_Head);

				Widget->OnClicked().AddLambda([Carousel, Nav, SquadMemberIndex]
				{
					for (const auto ChildWidget : Nav->GetAllChildren())
					{
						if (const auto Button = Cast<UUpSquadMemberNavButtonWidget>(ChildWidget))
						{
							// The base UE button widget will handle toggling the clicked button as selected,
							// so we don't need to do that ourselves.
							Button->SetIsSelected(false);
						}
					}

					Carousel->SetActiveWidgetIndex(SquadMemberIndex);
				});

				if (const auto Slot = Cast<UHorizontalBoxSlot>(Nav->AddChild(Widget)); Slot && SquadMemberIndex > 0)
				{
					Slot->SetPadding(FMargin(SquadMemberNavButtonGap, 0.f, 0.f, 0.f));
				}

				if (SquadMemberIndex <= 0) Widget->SetIsSelected(true);
			}

			SquadMemberIndex++;
		}
	}
}

void UUpSquadMenuWidget::PopulateSubMenuNav()
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

				SubMenuNavigationDelegate.Broadcast(SubMenuTag);
			});
			
			Nav->AddChild(Widget);

			if (WidgetIndex <= 0) Widget->SetIsSelected(true);
			
			WidgetIndex++;
		}
	}
}

void UUpSquadMenuWidget::SelectSquadMember()
{
	if (!SquadMemberTags.IsValidIndex(CurrentSquadMemberIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectSquadMember: Invalid squad member index %d"), CurrentSquadMemberIndex)
		return;
	}

	if (const auto Nav = GetSquadMembersNav())
	{
		uint8 WidgetIndex = 0;
		
		for (const auto ChildWidget : Nav->GetAllChildren())
		{
			if (const auto Button = Cast<UUpSquadMemberNavButtonWidget>(ChildWidget))
			{
				Button->SetIsSelected(WidgetIndex == CurrentSquadMemberIndex ? true : false);
			}

			WidgetIndex++;
		}
	}

	if (const auto Carousel = GetSquadMembersCarousel())
	{
		Carousel->SetActiveWidgetIndex(CurrentSquadMemberIndex);
	}
}

void UUpSquadMenuWidget::SelectSubMenu()
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
	
	SubMenuNavigationDelegate.Broadcast(SubMenuTags[CurrentSubMenuIndex]);
}
