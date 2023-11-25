// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpInventoriable.generated.h"

class UUpInventoryComponent;

UINTERFACE(MinimalAPI)
class UUpInventoriable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpInventoriable
{
	GENERATED_BODY()

public:
	virtual UUpInventoryComponent* GetInventoryComponent() const = 0;
};
