// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "UpMainMenuTabListWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMainMenuTabListWidget : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetTabsContainer() const;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpMenuTabData"))
	TObjectPtr<UDataTable> MenuTabDataTable;

	TScriptDelegate<FWeakObjectPtr> OnTabButtonCreationDelegate;
	TScriptDelegate<FWeakObjectPtr> OnTabButtonRemovalDelegate;

	UFUNCTION()
	void AddTabButton(FName TabId, UCommonButtonBase* TabButton);
	UFUNCTION()
	void RemoveTabButton(FName TabId, UCommonButtonBase* TabButton);
};
