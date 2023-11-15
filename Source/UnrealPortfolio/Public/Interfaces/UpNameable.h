// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpNameable.generated.h"

// `CannotImplementInterfaceInBlueprint` allows us to designate virtual functions as BlueprintCallable
// (see https://forums.unrealengine.com/t/how-do-i-expose-a-c-interface-to-blueprints-with-a-blueprintcallable-function/342082/2).
UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UUpNameable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpNameable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FText GetInGameName() const = 0;
};
