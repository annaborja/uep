// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpAttackAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAttackAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	void TriggerDamage(AActor* TargetActor) const;
};
