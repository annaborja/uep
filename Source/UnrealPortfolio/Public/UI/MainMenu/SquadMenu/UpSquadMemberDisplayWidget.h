// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpNpcCharacter.h"
#include "UI/UpCommonUserWidget.h"
#include "UpSquadMemberDisplayWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpSquadMemberInventoryMenuWidget;
class UUpSquadMemberStatsMenuWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void PopulateNpcData(const FUpNpcData& InNpcData);
	void SetActiveSubMenu(const FGameplayTag& SubMenuTag);

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpNpcData NpcData;

	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetSubMenuSwitcher() const;

	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberStatsMenuWidget* GetSquadMemberStatsMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberInventoryMenuWidget* GetSquadMemberInventoryMenu() const;
};
