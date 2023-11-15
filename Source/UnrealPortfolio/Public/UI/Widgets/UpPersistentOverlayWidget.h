// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/UpCommonUserWidget.h"
#include "UpPersistentOverlayWidget.generated.h"

class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentOverlayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void OpenMainMenu() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetContainerBase* GetMenuWidgetContainer() const;
	
	virtual void NativePreConstruct() override;

private:
	UFUNCTION()
	void OnDisplayedWidgetChanged(UCommonActivatableWidget* Widget) const;
};
