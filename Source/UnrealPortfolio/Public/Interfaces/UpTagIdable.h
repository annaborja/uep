// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpTagIdable.generated.h"

struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UUpTagIdable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpTagIdable
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetTagId() const = 0;
};
