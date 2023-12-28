// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpPlayableNpc.h"
#include "UI/UpCommonUserWidget.h"
#include "UpSquadMemberDisplayWidget.generated.h"

class UCommonActivatableWidgetSwitcher;
class UUpSquadMemberInventoryMenuWidget;
class UUpSquadMemberSkillsMenuWidget;
class UUpSquadMemberStatsMenuWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetNpc(AUpPlayableNpc* InNpc);
	void SetCharacterData(const FUpCharacterData InCharacterData);
	
	void SetActiveSubMenu(const FGameplayTag& SubMenuTag) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpCharacterData CharacterData;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableNpc> Npc;

	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetSubMenuSwitcher() const;

	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberStatsMenuWidget* GetSquadMemberStatsMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberInventoryMenuWidget* GetSquadMemberInventoryMenu() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberSkillsMenuWidget* GetSquadMemberSkillsMenu() const;

	void PopulateSubMenuData() const;
};
