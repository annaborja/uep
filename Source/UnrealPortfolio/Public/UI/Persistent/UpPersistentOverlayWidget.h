// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentOverlayWidget.generated.h"

class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;
class UUpCharacterSwitcherWidget;
class UUpDialogueOverlayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentOverlayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	bool IsDescendentWidgetActivated() const;
	void OpenMainMenu() const;
	UUpCharacterSwitcherWidget* OpenCharacterSwitcher() const;
	UUpDialogueOverlayWidget* OpenDialogueFlow() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetCharacterSwitcherContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetDialogueWidgetContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetMainMenuWidgetContainer() const;

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetCrosshairVisibility() const;
	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetInteractionDisplayVisibility() const;
	
	virtual void NativePreConstruct() override;

private:
	UFUNCTION()
	void OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget) const;
};
