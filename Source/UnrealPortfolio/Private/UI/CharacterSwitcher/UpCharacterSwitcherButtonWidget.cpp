// Copyright AB. All Rights Reserved.

#include "UI/CharacterSwitcher/UpCharacterSwitcherButtonWidget.h"

#include "Characters/UpNpcCharacter.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpCharacterSwitcherButtonWidget::SetNpc(AUpNpcCharacter* InNpc)
{
	Npc = InNpc;

	if (Npc)
	{
		Label = Npc->GetInGameName();

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
	CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Empty;
	SetIsEnabled(false);
}

void UUpCharacterSwitcherButtonWidget::SetNpcTag(const FGameplayTag& NpcTag)
{
	Npc = nullptr;
	Label = UUpBlueprintFunctionLibrary::GetInGameName(this, NpcTag);
	CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Unavailable;
	SetIsEnabled(false);
}

void UUpCharacterSwitcherButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsEnabled(false);
	OnClicked().Clear();
}
