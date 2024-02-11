// Copyright AB. All Rights Reserved.

#include "UI/PowerWheel/UpPowerWheelButtonWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpPowerWheelButtonWidget::SetNpc(AUpPlayableNpc* InNpc)
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
				PowerWheelButtonState = EUpPowerWheelButtonState::Active;
				SetIsEnabled(false);
				
				return;
			}
		}
		
		PowerWheelButtonState = EUpPowerWheelButtonState::Available;
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
	PowerWheelButtonState = EUpPowerWheelButtonState::Empty;
	SetIsEnabled(false);
}

void UUpPowerWheelButtonWidget::SetNpcTag(const FGameplayTag& NpcTag)
{
	Npc = nullptr;
	PowerWheelButtonState = EUpPowerWheelButtonState::Unavailable;
	SetIsEnabled(false);
	
	FUpCharacterData CharacterData;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		CharacterData = GameInstance->GetCharacterData(NpcTag);
	}
	
	Label = CharacterData.Name;
	Image = CharacterData.Image_Head;
}

void UUpPowerWheelButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsEnabled(false);
	OnClicked().Clear();
}
