// Copyright AB. All Rights Reserved.

#include "UI/PowerWheel/UpPowerWheelWidget.h"

#include "CommonButtonBase.h"
#include "EnhancedInputSubsystems.h"
#include "UpGameInstance.h"
#include "UI/UpHud.h"
#include "UI/PowerWheel/UpPowerWheelButtonWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpPowerWheelWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulatePowerWheelButtons();
}

FReply UUpPowerWheelWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (CustomHud)
	{
		if (const auto CustomController = CustomHud->GetCustomController())
		{
			if (const auto InputAction = CustomController->GetInputAction_ClosePowerWheel())
			{
				if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CustomController->GetLocalPlayer());
					Subsystem && Subsystem->QueryKeysMappedToAction(InputAction).Contains(InKeyEvent.GetKey()))
				{
					CustomController->ClosePowerWheel();
				}
			}
		}
	}

	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UUpPowerWheelWidget::PopulatePowerWheelButtons() const
{
	const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
	const auto AllSpecialMoveData = GameInstance ? GameInstance->GetAllSpecialMoveData() : TArray<FUpSpecialMoveData>();
	const auto PowerWheelButtons = GetPowerWheelButtons();

	for (uint8 i = 0; PowerWheelButtons.IsValidIndex(i); i++)
	{
		if (const auto Button = PowerWheelButtons[i])
		{
			Button->SetSpecialMoveData(AllSpecialMoveData.IsValidIndex(i) ? AllSpecialMoveData[i] : FUpSpecialMoveData());
		}
	}
}
