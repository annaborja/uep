// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentSquadDisplayWidget.h"

#include "CommonActionWidget.h"
#include "UI/UpHud.h"
#include "UI/Persistent/UpPersistentVitalStatsDisplayWidget.h"

void UUpPersistentSquadDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->PossessedCharacterDelegate.AddUObject(this, &ThisClass::HandlePossessedCharacterChange);
	CustomHud->SecondarySquadMemberDelegate.AddUObject(this, &ThisClass::HandleSecondarySquadMemberBroadcast);

	const auto Widgets = GetSecondaryDisplayWidgets();
	
	for (uint8 i = 0; i < Widgets.Num(); i++)
	{
		if (!Widgets.IsValidIndex(i)) continue;

		if (const auto Widget = Widgets[i])
		{
			Widget->SetSecondary(true);

			if (SquadMemberCommonInputActions.IsValidIndex(i))
			{
				if (const auto CommonActionWidget = Widget->GetCommonActionWidget())
				{
					CommonActionWidget->SetInputAction(SquadMemberCommonInputActions[i]);
				}
			}
		}
	}
}

void UUpPersistentSquadDisplayWidget::HandlePossessedCharacterChange(AUpPlayableCharacter* PossessedCharacter)
{
	UUpPersistentVitalStatsDisplayWidget* PrevSecondaryWidget = nullptr;
	
	for (const auto Widget : GetSecondaryDisplayWidgets())
	{
		if (Widget->GetCharacter() == PossessedCharacter)
		{
			PrevSecondaryWidget = Widget;
			break;
		}
	}
	
	if (const auto Widget = GetVitalStatsDisplay_Main())
	{
		if (const auto PrevMainCharacter = Widget->GetCharacter(); PrevSecondaryWidget && PrevMainCharacter)
		{
			PrevSecondaryWidget->SetCharacter(PrevMainCharacter);
		}
		
		Widget->SetCharacter(PossessedCharacter);
	}
}

void UUpPersistentSquadDisplayWidget::HandleSecondarySquadMemberBroadcast(AUpPlayableCharacter* Character)
{
	for (const auto Widget : GetSecondaryDisplayWidgets())
	{
		if (!Widget->GetCharacter())
		{
			Widget->SetCharacter(Character);
			return;
		}
	}
}

TArray<UUpPersistentVitalStatsDisplayWidget*> UUpPersistentSquadDisplayWidget::GetSecondaryDisplayWidgets()
{
	TArray<UUpPersistentVitalStatsDisplayWidget*> Widgets;

	for (const auto Widget : TArray { GetVitalStatsDisplay_Secondary1(), GetVitalStatsDisplay_Secondary2() })
	{
		if (Widget) Widgets.Add(Widget);
	}

	return Widgets;
}
