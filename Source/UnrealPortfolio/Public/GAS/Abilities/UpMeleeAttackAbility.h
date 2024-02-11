// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpMeleeAttackAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMeleeAttackAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpMeleeAttackAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnMontageCompleted();
};
