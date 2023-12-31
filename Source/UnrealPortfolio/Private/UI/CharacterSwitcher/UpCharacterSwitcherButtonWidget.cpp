// Copyright AB. All Rights Reserved.

#include "UI/CharacterSwitcher/UpCharacterSwitcherButtonWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpCharacterSwitcherButtonWidget::SetNpc(AUpPlayableNpc* InNpc)
{
	Npc = InNpc;

	if (Npc)
	{
		Label = Npc->GetInGameName();
		Image = Npc->GetCharacterData().Image_Head;

		if (CustomHud)
		{
			if (const auto CustomController = CustomHud->GetCustomController(); CustomController && CustomController->GetPossessedCharacter() == Npc)
			{
				CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Active;
				SetIsEnabled(false);
				
				return;
			}
		}
		
		CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Available;
		SetIsEnabled(true);

		OnClicked().Clear();
		OnClicked().AddLambda([this]
		{
			if (!CustomHud || !Npc) return;

			CustomHud->SwitchCharacter(Npc);
		});
		
		return;
	}
	
	Label = FText();
	Image = nullptr;
	CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Empty;
	SetIsEnabled(false);
}

void UUpCharacterSwitcherButtonWidget::SetNpcTag(const FGameplayTag& NpcTag)
{
	Npc = nullptr;
	CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Unavailable;
	SetIsEnabled(false);
	
	FUpCharacterData CharacterData;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		CharacterData = GameInstance->GetCharacterData(NpcTag);
	}
	
	Label = CharacterData.Name;
	Image = CharacterData.Image_Head;
}

void UUpCharacterSwitcherButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsEnabled(false);
	OnClicked().Clear();
}
