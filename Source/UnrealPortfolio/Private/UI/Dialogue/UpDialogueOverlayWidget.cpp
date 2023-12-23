// Copyright AB. All Rights Reserved.

#include "UI/Dialogue/UpDialogueOverlayWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "Characters/UpPlayableNpc.h"
#include "UI/Dialogue/UpDialogueOptionsDisplayWidget.h"
#include "UI/Dialogue/UpDialogueStepDisplayWidget.h"

void UUpDialogueOverlayWidget::DisplayDialogueStep(AUpPlayableNpc* Npc, const FUpDialogueStepData& DialogueStep) const
{
	if (const auto DialogueStepDisplay = GetDialogueStepDisplay())
	{
		DialogueStepDisplay->DisplayDialogueStep(Npc, DialogueStep);
	}
	
	ActivateDialogueStepDisplay();
}

void UUpDialogueOverlayWidget::DisplayDialogueOptions(AUpPlayableNpc* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions) const
{
	if (const auto DialogueOptionsDisplay = GetDialogueOptionsDisplay())
	{
		DialogueOptionsDisplay->PopulateDialogueOptions(Npc, DialogueOptions);

		if (const auto WidgetSwitcher = GetWidgetSwitcher())
		{
			WidgetSwitcher->SetActiveWidget(DialogueOptionsDisplay);
		}
	}
}

void UUpDialogueOverlayWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	ActivateDialogueStepDisplay();
}

void UUpDialogueOverlayWidget::ActivateDialogueStepDisplay() const
{
	if (const auto WidgetSwitcher = GetWidgetSwitcher())
	{
		if (const auto DialogueStepDisplay = GetDialogueStepDisplay())
		{
			WidgetSwitcher->SetActiveWidget(DialogueStepDisplay);
		}
	}
}
