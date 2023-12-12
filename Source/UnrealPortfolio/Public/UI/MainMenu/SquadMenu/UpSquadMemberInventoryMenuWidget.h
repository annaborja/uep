// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpNpcCharacter.h"
#include "Components/UpInventoryComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpSquadMemberInventoryMenuWidget.generated.h"

class AUpNpcCharacter;
class UUpSquadMemberInventoryItemDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberInventoryMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetNpc(AUpNpcCharacter* InNpc);
	void SetNpcTagId(const FGameplayTag& NpcTagId);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetInventoryItemsContainer() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpSquadMemberInventoryItemDisplayWidget> InventoryItemDisplayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float InventoryItemGap = 16.f;

	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> Npc;

	FUpInventory Inventory;

	void PopulateInventory(const FGameplayTag& NpcTagId);
	void PopulateInventoryItems();
};
