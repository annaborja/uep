// Copyright AB. All Rights Reserved.

#include "UI/CharacterSwitcher/UpCharacterSwitcherWidget.h"

#include "EnhancedInputSubsystems.h"
#include "Characters/Player/UpPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UUpCharacterSwitcherWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	bCanClose = false;
}

void UUpCharacterSwitcherWidget::HandleAnyKeyReleased(const FKey& Key)
{
	if (const auto PlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (const auto OpenCharacterSwitcherInputAction = PlayerController->GetOpenCharacterSwitcherInputAction())
		{
			if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
				Subsystem && Subsystem->QueryKeysMappedToAction(OpenCharacterSwitcherInputAction).Contains(Key))
			{
				// HACK: Without this boolean guard, we'll see the UI flash once before remaining open.
				if (bCanClose)
				{
					PlayerController->CloseCharacterSwitcher();
				} else
				{
					bCanClose = true;
				}
			}
		}
	}
}
