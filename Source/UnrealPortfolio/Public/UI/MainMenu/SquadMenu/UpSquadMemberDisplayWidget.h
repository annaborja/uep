// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpNpcCharacter.h"
#include "UI/UpCommonUserWidget.h"
#include "UpSquadMemberDisplayWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpSquadMemberInventoryMenuWidget;
class UUpSquadMemberStatsMenuWidget;
class UUpTabListWidget;
class UUpTabWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void PopulateNpcData(const FUpNpcData& InNpcData);

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpNpcData NpcData;

	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	UUpTabListWidget* GetTabList() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetTabContentSwitcher() const;

	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberStatsMenuWidget* GetSquadMemberStatsMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberInventoryMenuWidget* GetSquadMemberInventoryMenu() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpTabWidget> TabClass;
};
