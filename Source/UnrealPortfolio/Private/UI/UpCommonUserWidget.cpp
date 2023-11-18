// Copyright AB. All Rights Reserved.

#include "UI/UpCommonUserWidget.h"

void UUpCommonUserWidget::SetCustomHud(AUpHud* InCustomHud)
{
	if (CustomHud == InCustomHud) return;

	CustomHud = InCustomHud;
	OnCustomHudSet(CustomHud);
}

void UUpCommonUserWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	for (const auto Widget : GetHudSettableChildren())
	{
		if (const auto HudSettable = Cast<IUpHudSettable>(Widget))
		{
			HudSettable->SetCustomHud(CustomHud);
		}
	}
}
