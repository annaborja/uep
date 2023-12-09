// Copyright AB. All Rights Reserved.

#include "UI/CharacterSwitcher/UpCharacterSwitcherButtonWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpNpcCharacter.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpCharacterSwitcherButtonWidget::SetNpc(AUpNpcCharacter* InNpc)
{
	Npc = InNpc;

	if (Npc)
	{
		Label = Npc->GetInGameName();
		Image = Npc->GetImage_Head();

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
	
	FUpNpcData NpcData;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		NpcData = GameInstance->GetNpcData(NpcTag);
	}
	
	Label = NpcData.Name;
	Image = NpcData.Image_Head;
}

void UUpCharacterSwitcherButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsEnabled(false);
	OnClicked().Clear();
}
