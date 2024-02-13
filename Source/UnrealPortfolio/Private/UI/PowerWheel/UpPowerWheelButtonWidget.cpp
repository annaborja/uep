// Copyright AB. All Rights Reserved.

#include "UI/PowerWheel/UpPowerWheelButtonWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpPowerWheelButtonWidget::SetSpecialMoveData(const FUpSpecialMoveData InSpecialMoveData)
{
	SpecialMoveData = InSpecialMoveData;

	if (SpecialMoveData.IsValid())
	{
		PowerWheelButtonState = EUpPowerWheelButtonState::Available;
		
		if (CustomHud)
		{
			if (const auto CustomController = CustomHud->GetCustomController())
			{
				if (const auto PossessedCharacter = CustomController->GetPossessedCharacter();
					PossessedCharacter && PossessedCharacter->GetActiveSpecialMoveTag().MatchesTagExact(SpecialMoveData.TagId))
				{
					PowerWheelButtonState = EUpPowerWheelButtonState::Active;
				}
			}
		}
		
		SetIsEnabled(true);

		OnClicked().Clear();
		OnClicked().AddLambda([this]
		{
			SetFocus();
			
			if (!CustomHud) return;

			if (const auto Controller = CustomHud->GetCustomController())
			{
				Controller->ClosePowerWheel();
			}
		});
	} else
	{
		PowerWheelButtonState = EUpPowerWheelButtonState::Empty;
		SetIsEnabled(false);
	}
}

void UUpPowerWheelButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsEnabled(false);
	OnClicked().Clear();
}

void UUpPowerWheelButtonWidget::NativeOnHovered()
{
	Super::NativeOnHovered();

	SetFocus();
}

FReply UUpPowerWheelButtonWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (Sfx_Focus) UGameplayStatics::PlaySound2D(this, Sfx_Focus);
	
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}
