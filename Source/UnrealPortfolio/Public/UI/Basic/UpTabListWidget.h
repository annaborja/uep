// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "UpTabListWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpTabListWidget : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	virtual void HandleTabRemoval_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetTabsContainer() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpMenuTabData"))
	TObjectPtr<UDataTable> MenuTabDataTable;
};
