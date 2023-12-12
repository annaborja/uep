// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpInventoryComponent.h"
#include "UObject/Interface.h"
#include "UpCharacterEquippable.generated.h"

UINTERFACE(MinimalAPI)
class UUpCharacterEquippable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpCharacterEquippable
{
	GENERATED_BODY()

public:
	virtual FUpCharacterEquipment GetCharacterEquipment() const = 0;
};
