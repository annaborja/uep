// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpNameable.generated.h"

UINTERFACE(MinimalAPI)
class UUpNameable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpNameable
{
	GENERATED_BODY()

public:
	virtual FText GetInGameName() const = 0;
};
