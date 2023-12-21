// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpAttackAbility.h"
#include "UpGunFireAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpGunFireAbility : public UUpAttackAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UParticleSystem> ImpactParticleSystem;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Debug")
	bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LineTraceLength = 10000.f;
	
	virtual void HandleRepeatAction(int32 ActionNumber) override;
};
