// Copyright AB. All Rights Reserved.

#include "UI/Dialogue/UpDialogueOverlayWidget.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UI/Dialogue/UpDialogueStepDisplayWidget.h"

void UUpDialogueOverlayWidget::DisplayDialogueStep(AUpNpcCharacter* Npc, const FUpDialogueStepData& DialogueStep) const
{
	if (const auto DialogueStepDisplay = GetDialogueStepDisplay())
	{
		DialogueStepDisplay->DisplayDialogueStep(Npc, DialogueStep);
	}
	
	ActivateDialogueStepDisplay();
}

void UUpDialogueOverlayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	OnDeactivated().AddUObject(this, &ThisClass::ActivateDialogueStepDisplay);
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
