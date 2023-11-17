// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentOverlayWidget.generated.h"

class UUpDialogueOverlayWidget;
class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentOverlayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void OpenMainMenu() const;
	UUpDialogueOverlayWidget* OpenDialogueFlow() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetDialogueWidgetContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetMenuWidgetContainer() const;

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetTargetInteractableDisplayVisibility() const;
	
	virtual void NativePreConstruct() override;

private:
	UFUNCTION()
	void OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget) const;
	
	bool IsDescendentWidgetActivated() const;
};
