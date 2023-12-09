// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/SquadMenu/UpSquadMenuWidget.h"

#include "UpGameInstance.h"
#include "CommonWidgetCarousel.h"
#include "Characters/UpNpcCharacter.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/PanelWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberDisplayWidget.h"
#include "UI/MainMenu/SquadMenu/UpSquadMemberNavButtonWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpSquadMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PartyMemberTags.Reset();
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->GetPartyMemberTags(PartyMemberTags);
	}
	
	PopulateSquadMembers();
}

void UUpSquadMenuWidget::PopulateSquadMembers()
{
	if (const auto Carousel = GetSquadMembersCarousel())
	{
		Carousel->ClearChildren();

		TArray<FGameplayTag> NpcTags;
		PartyMemberTags.GetGameplayTagArray(NpcTags);

		// TODO(P0): Sorting doesn't seem to be working for some reason.
		NpcTags.Sort([this] (const FGameplayTag& TagA, const FGameplayTag& TagB)
		{
			return UUpBlueprintFunctionLibrary::GetInGameName(this, TagA).ToString().Compare(
				UUpBlueprintFunctionLibrary::GetInGameName(this, TagB).ToString());
		});

		const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
		const auto Nav = GetSquadMembersNav();
		uint8 NpcIndex = 0;

		if (Nav) Nav->ClearChildren();
		
		for (const auto& NpcTag : NpcTags)
		{
			FUpNpcData NpcData;
			
			if (GameInstance)
			{
				NpcData = GameInstance->GetNpcData(NpcTag);
			}
			
			if (SquadMemberDisplayWidgetClass)
			{
				const auto Widget = CreateWidget<UUpSquadMemberDisplayWidget>(this, SquadMemberDisplayWidgetClass);

				if (NpcData.IsValid()) Widget->PopulateNpcData(NpcData);

				Carousel->AddChild(Widget);
			}

			if (SquadMemberNavButtonClass && Nav)
			{
				const auto Widget = CreateWidget<UUpSquadMemberNavButtonWidget>(this, SquadMemberNavButtonClass);

				if (NpcData.IsValid()) Widget->SetImage(NpcData.Image_Head);

				const auto NavigateCarousel = [Carousel, NpcIndex]
				{
					Carousel->SetActiveWidgetIndex(NpcIndex);
				};
				
				Widget->OnClicked().AddLambda(NavigateCarousel);
				Widget->OnFocusReceived().AddLambda(NavigateCarousel);

				if (const auto Slot = Cast<UHorizontalBoxSlot>(Nav->AddChild(Widget)); Slot && NpcIndex > 0)
				{
					Slot->SetPadding(FMargin(SquadMemberNavButtonGap, 0.f, 0.f, 0.f));
				}

				if (NpcIndex == 0) Widget->SetFocus();
			}

			NpcIndex++;
		}
	}
}
