// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpCombatable.generated.h"

class UUpCombatComponent;

UINTERFACE(MinimalAPI)
class UUpCombatable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpCombatable
{
	GENERATED_BODY()

public:
	virtual UUpCombatComponent* GetCombatComponent() const = 0;
	virtual uint8 GetCombatLevel() const { return 1; }
	virtual UAnimMontage* GetHitReactionsMontage() const = 0;
};
