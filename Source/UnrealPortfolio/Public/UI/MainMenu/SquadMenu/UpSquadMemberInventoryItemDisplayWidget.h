// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpInventoryComponent.h"
#include "UI/UpCommonButtonBase.h"
#include "UpSquadMemberInventoryItemDisplayWidget.generated.h"

class AUpNpcCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberInventoryItemDisplayWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void PopulateInventoryItemData(const FGameplayTag& ItemTagId, const FUpInventoryItemData& InventoryItemData, AUpNpcCharacter* Npc);

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpItemData ItemData;
};
