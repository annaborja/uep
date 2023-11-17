// Copyright AB. All Rights Reserved.

#include "UI/Dialogue/UpDialogueOptionButtonWidget.h"

#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpDialogueOptionButtonWidget::SetDialogueOption(const FUpDialogueOptionData& InDialogueOption)
{
	DialogueOption = InDialogueOption;
	DialogueOptionText = UUpBlueprintFunctionLibrary::GetInGameNameifiedText(this, DialogueOption.Text);
}

void UUpDialogueOptionButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	OnClicked().AddLambda([this]
	{
		if (!CustomHud) return;

		CustomHud->SelectDialogueOption(Npc, DialogueOption);
	});
}
