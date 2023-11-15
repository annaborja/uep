// Copyright AB. All Rights Reserved.

#include "UI/UpHud.h"

#include "UI/Widgets/UpCommonActivatableWidget.h"
#include "UI/Widgets/UpPersistentOverlayWidget.h"

void AUpHud::Init(AUpPlayerController* InPlayerController)
{
	check(MenuSwitcherClass);
	check(PersistentOverlayClass);
	
	PlayerController = InPlayerController;

	// Initialize the persistent overlay.
	if (const auto World = GetWorld(); World && PersistentOverlayClass)
	{
		PersistentOverlayWidget = CreateWidget<UUpPersistentOverlayWidget>(World, PersistentOverlayClass);
		PersistentOverlayWidget->SetCustomHud(this);
		PersistentOverlayWidget->AddToViewport();
	}
}

void AUpHud::OpenMainMenu() const
{
	if (!PersistentOverlayWidget) return;
	
	PersistentOverlayWidget->OpenMainMenu();
}
