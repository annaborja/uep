// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpInventoryComponent.h"
#include "UI/UpCommonButtonBase.h"
#include "UpSquadMemberInventoryItemDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberInventoryItemDisplayWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void PopulateInventoryItemData(const FUpInventoryItemData& InventoryItemData);

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpItemData ItemData;
};
