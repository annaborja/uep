// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Tags/UiTags.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpJournalMenuWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpCodexMenuWidget;
class UUpObjectivesMenuWidget;
class UUpSquadMemberSubMenuNavButtonWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpJournalMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnActivated() override;
	
	void SetActiveSubMenu(const FGameplayTag& SubMenuTag) const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetSubMenuNav() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetSubMenuSwitcher() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpCodexMenuWidget* GetCodexMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpObjectivesMenuWidget* GetObjectivesMenu() const;
	
	UFUNCTION(BlueprintCallable)
	void ShowNextSubMenu();
	UFUNCTION(BlueprintCallable)
	void ShowPreviousSubMenu();
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpMenuTabData"))
	TObjectPtr<UDataTable> MenuTabDataTable;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberSubMenuNavButtonWidget> SubMenuNavButtonClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USoundBase> Sfx_SubMenuSwitch;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	TArray<FGameplayTag> SubMenuTags { TAG_Menu_Journal_Objectives, TAG_Menu_Journal_Codex };
	
	uint8 CurrentSubMenuIndex = 0;
	
	void PopulateSubMenuNav();
	void SelectSubMenu();
};
