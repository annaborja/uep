// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpMainMenuSwitcherWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpInventoryMenuWidget;
class UUpJournalMenuWidget;
class UUpQuestsMenuWidget;
class UUpSettingsMenuWidget;
class UUpSquadMenuWidget;
class UUpStatsMenuWidget;
class UUpTabListWidget;
class UUpTabWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpMainMenuSwitcherWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UUpTabListWidget* GetTabList() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetWidgetSwitcher() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUpInventoryMenuWidget* GetInventoryMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpJournalMenuWidget* GetJournalMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpQuestsMenuWidget* GetQuestsMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSettingsMenuWidget* GetSettingsMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMenuWidget* GetSquadMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpStatsMenuWidget* GetStatsMenu() const;
	
	UFUNCTION(BlueprintCallable)
	void HandleCloseMenuAction();
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void HandleQuitGameAction() const;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpTabWidget> MenuTabClass;
	
	void ResumeGame();
	void SetUpTabList();
};
