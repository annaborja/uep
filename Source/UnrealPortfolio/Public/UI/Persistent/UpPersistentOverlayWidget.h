// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentOverlayWidget.generated.h"

class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;
class UUpPowerWheelWidget;
class UUpDialogueOverlayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentOverlayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	bool IsDescendentWidgetActivated() const;
	bool IsMainMenuOpen() const;
	void OpenMainMenu() const;
	UUpPowerWheelWidget* OpenPowerWheel() const;
	UUpDialogueOverlayWidget* OpenDialogueFlow() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetPowerWheelContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetDialogueWidgetContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetMainMenuWidgetContainer() const;

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetInteractionDisplayVisibility() const;
	
	virtual void NativePreConstruct() override;

private:
	UFUNCTION()
	void OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget) const;
};
