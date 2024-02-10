// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpMainMenuSwitcherWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpJournalMenuWidget;
class UUpSettingsMenuWidget;
class UUpSquadMenuWidget;
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
	UUpJournalMenuWidget* GetJournalMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSettingsMenuWidget* GetSettingsMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMenuWidget* GetSquadMenu() const;
	
	UFUNCTION(BlueprintCallable)
	void HandleCloseMenuAction();
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void HandleQuitGameAction() const;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpTabWidget> MenuTabClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USoundBase> Sfx_MainMenuOpen;
	
	void ResumeGame() const;
	void SetUpTabList() const;
};
