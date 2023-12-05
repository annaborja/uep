// Copyright AB. All Rights Reserved.

#include "UI/UpCommonActivatableWidget.h"

#include "UI/UpHud.h"

TOptional<FUIInputConfig> UUpCommonActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputMode)
	{
		// If we're in All input mode, we assume that we want to enable mouse usage and visibility.
	case ECommonInputMode::All:
		// In Menu input mode, UE intentionally registers game inputs if mouse capture is enabled.
	case ECommonInputMode::Menu:
		return FUIInputConfig(InputMode, EMouseCaptureMode::NoCapture);
	default:
		return FUIInputConfig(InputMode, EMouseCaptureMode::CapturePermanently);
	}
}

void UUpCommonActivatableWidget::SetCustomHud(AUpHud* InCustomHud)
{
	if (CustomHud == InCustomHud) return;
	
	CustomHud = InCustomHud;
	OnCustomHudSet(CustomHud);
}

void UUpCommonActivatableWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	for (const auto Widget : GetHudSettableChildren())
	{
		if (const auto HudSettable = Cast<IUpHudSettable>(Widget))
		{
			HudSettable->SetCustomHud(CustomHud);
		}
	}
}
