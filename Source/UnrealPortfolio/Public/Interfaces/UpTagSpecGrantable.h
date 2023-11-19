// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpTagSpecGrantable.generated.h"

struct FUpTagSpec;

UINTERFACE(MinimalAPI)
class UUpTagSpecGrantable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpTagSpecGrantable
{
	GENERATED_BODY()
	
public:
	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) = 0;
};
