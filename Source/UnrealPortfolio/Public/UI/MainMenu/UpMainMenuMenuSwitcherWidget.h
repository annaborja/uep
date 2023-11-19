// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpMainMenuMenuSwitcherWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpMainMenuTabListWidget;
class UUpMainMenuTabWidget;
class UUpReputationMenuWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpMainMenuMenuSwitcherWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UUpMainMenuTabListWidget* GetTabList() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetWidgetSwitcher() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUpReputationMenuWidget* GetReputationMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidget* GetSettingsMenu() const;
	
	UFUNCTION(BlueprintCallable)
	void HandleCloseMenuAction();
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void HandleQuitGameAction() const;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpMainMenuTabWidget> MenuTabClass;

	bool bDelegatesAdded = false;

	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void SetUpTabList();
};
