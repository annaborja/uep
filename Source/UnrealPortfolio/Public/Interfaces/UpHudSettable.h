// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpHudSettable.generated.h"

class AUpHud;

UINTERFACE(MinimalAPI)
class UUpHudSettable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpHudSettable
{
	GENERATED_BODY()

public:
	virtual void SetCustomHud(AUpHud* InCustomHud) = 0;
};
