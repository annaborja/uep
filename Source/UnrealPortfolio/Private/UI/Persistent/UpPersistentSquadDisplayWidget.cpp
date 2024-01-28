// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentSquadDisplayWidget.h"

#include "UI/UpHud.h"
#include "UI/Persistent/UpPersistentVitalStatsDisplayWidget.h"

void UUpPersistentSquadDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->PossessedCharacterDelegate.AddUObject(this, &ThisClass::HandlePossessedCharacterChange);
	CustomHud->SecondarySquadMemberDelegate.AddUObject(this, &ThisClass::HandleSecondarySquadMemberBroadcast);

	for (const auto Widget : GetSecondaryDisplayWidgets())
	{
		Widget->SetSecondary(true);
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
