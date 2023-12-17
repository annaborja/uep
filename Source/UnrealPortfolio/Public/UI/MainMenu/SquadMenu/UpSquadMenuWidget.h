// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Tags/UiTags.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpSquadMenuWidget.generated.h"

class UCommonWidgetCarousel;
class UDataTable;
class UUpSquadMemberDisplayWidget;
class UUpSquadMemberNavButtonWidget;
class UUpSquadMemberSubMenuNavButtonWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpSquadMenuWidgetSubMenuNavigationDelegate, const FGameplayTag);

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnActivated() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UCommonWidgetCarousel* GetSquadMembersCarousel() const;
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetSquadMembersNav() const;
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetSubMenuNav() const;
	
	UFUNCTION(BlueprintCallable)
	void ShowNextSquadMember();
	UFUNCTION(BlueprintCallable)
	void ShowPreviousSquadMember();
	
	UFUNCTION(BlueprintCallable)
	void ShowNextSubMenu();
	UFUNCTION(BlueprintCallable)
	void ShowPreviousSubMenu();

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpMenuTabData"))
	TObjectPtr<UDataTable> MenuTabDataTable;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberDisplayWidget> SquadMemberDisplayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberNavButtonWidget> SquadMemberNavButtonClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberSubMenuNavButtonWidget> SubMenuNavButtonClass;

	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float SquadMemberNavButtonGap = 12.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	TArray<FGameplayTag> SubMenuTags { TAG_Menu_SquadMember_Stats, TAG_Menu_SquadMember_Inventory, TAG_Menu_SquadMember_Skills };

	uint8 CurrentSquadMemberIndex = 0;
	TArray<FGameplayTag> SquadMemberTags;
	
	uint8 CurrentSubMenuIndex = 0;
	FUpSquadMenuWidgetSubMenuNavigationDelegate SubMenuNavigationDelegate;

	void PopulateSquadMembers();
	void PopulateSubMenuNav();
	
	void SelectSquadMember();
	void SelectSubMenu();
};
