// Copyright AB. All Rights Reserved.

#include "UI/Dialogue/UpDialogueOptionsDisplayWidget.h"

#include "Characters/UpPlayableNpc.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "UI/Dialogue/UpDialogueOptionButtonWidget.h"

void UUpDialogueOptionsDisplayWidget::PopulateDialogueOptions(AUpPlayableNpc* Npc, TArray<FUpDialogueOptionData> DialogueOptions)
{
	if (const auto DialogueOptionsContainer = GetDialogueOptionsContainer(); DialogueOptionsContainer && DialogueOptionButtonClass)
	{
		DialogueOptionsContainer->ClearChildren();
		uint8 DialogueOptionIndex = 0;

		for (const auto& DialogueOption : DialogueOptions)
		{
			const auto DialogueOptionButton = CreateWidget<UUpDialogueOptionButtonWidget>(this, DialogueOptionButtonClass);
			DialogueOptionButton->SetCustomHud(CustomHud);
			DialogueOptionButton->SetDialogueOption(DialogueOption);
			DialogueOptionButton->SetNpc(Npc);

			if (const auto PanelSlot = Cast<UVerticalBoxSlot>(DialogueOptionsContainer->AddChild(DialogueOptionButton));
				PanelSlot && DialogueOptionIndex > 0)
			{
				PanelSlot->SetPadding(FMargin(0.f, DialogueOptionGap, 0.f, 0.f));
			}

			DialogueOptionIndex++;
		}
	}
}

UWidget* UUpDialogueOptionsDisplayWidget::NativeGetDesiredFocusTarget() const
{
	if (const auto DialogueOptionsContainer = GetDialogueOptionsContainer())
	{
		return DialogueOptionsContainer->GetChildAt(0);
	}
	
	return Super::NativeGetDesiredFocusTarget();
}
