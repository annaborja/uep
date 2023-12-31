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
	virtual void Die() = 0;
	virtual UUpCombatComponent* GetCombatComponent() const = 0;
	virtual uint8 GetCombatLevel() const { return 1; }
	virtual UAnimMontage* GetHitReactionsMontage() const = 0;
	virtual UAnimMontage* GetReloadsMontage() const = 0;
};
