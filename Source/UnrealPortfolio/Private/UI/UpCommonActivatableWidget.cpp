// Copyright AB. All Rights Reserved.

#include "UI/UpCommonActivatableWidget.h"

#include "UI/UpHud.h"

TOptional<FUIInputConfig> UUpCommonActivatableWidget::GetDesiredInputConfig() const
{
	if (InputMode == ECommonInputMode::Menu)
	{
		// In Menu input mode, UE intentionally registers game inputs if mouse capture is enabled.
		return FUIInputConfig(InputMode, EMouseCaptureMode::NoCapture);
	}
	
	return FUIInputConfig(InputMode, EMouseCaptureMode::CapturePermanently);
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
