// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpSquadMenuWidget.generated.h"

class UCommonWidgetCarousel;
class UUpSquadMemberDisplayWidget;
class UUpSquadMemberNavButtonWidget;

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

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberDisplayWidget> SquadMemberDisplayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberNavButtonWidget> SquadMemberNavButtonClass;

	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float SquadMemberNavButtonGap = 8.f;

	FGameplayTagContainer PartyMemberTags;

	void PopulateSquadMembers();
};
