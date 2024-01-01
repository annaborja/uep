// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "Utils/Structs.h"
#include "UpSquadMemberEquipmentItemDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberEquipmentItemDisplayWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void PopulateEquipmentItemData(const FGameplayTag& ItemTagId);

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpItemData ItemData;
};
