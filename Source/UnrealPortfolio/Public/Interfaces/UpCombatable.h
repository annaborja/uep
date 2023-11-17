// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpCombatable.generated.h"

UINTERFACE(MinimalAPI)
class UUpCombatable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpCombatable
{
	GENERATED_BODY()

public:
	virtual uint8 GetCombatLevel() const { return 1; }
};
