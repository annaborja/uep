// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interfaces/UpTagIdable.h"
#include "UpGameplayAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpGameplayAbility : public UGameplayAbility, public IUpTagIdable
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetTagId() const override { return TagId; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
};
