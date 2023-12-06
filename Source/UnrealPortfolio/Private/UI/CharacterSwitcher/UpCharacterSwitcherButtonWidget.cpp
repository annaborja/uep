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
		CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Available;
		Label = Npc->GetInGameName();
		SetIsEnabled(true);
	} else
	{
		CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Empty;
		Label = FText();
		SetIsEnabled(false);
	}
}

void UUpCharacterSwitcherButtonWidget::SetNpcTag(const FGameplayTag& NpcTag)
{
	Npc = nullptr;
	CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Unavailable;
	Label = UUpBlueprintFunctionLibrary::GetInGameName(this, NpcTag);
	
	SetIsEnabled(false);
}

void UUpCharacterSwitcherButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsEnabled(false);

	OnClicked().AddLambda([this]
	{
		if (!CustomHud || !Npc) return;

		CustomHud->SwitchCharacter(Npc);
	});
}
