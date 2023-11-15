// Copyright AB. All Rights Reserved.

#include "UI/Widgets/UpCommonUserWidget.h"

void UUpCommonUserWidget::SetCustomHud(AUpHud* InCustomHud)
{
	if (CustomHud == InCustomHud) return;

	CustomHud = InCustomHud;
	OnCustomHudSet();
}

void UUpCommonUserWidget::OnCustomHudSet_Implementation()
{
	if (!CustomHud) return;

	for (const auto Widget : GetHudSettableChildren())
	{
		if (const auto HudSettable = Cast<IUpHudSettable>(Widget))
		{
			HudSettable->SetCustomHud(CustomHud);
		}
	}
}
