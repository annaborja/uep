// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpSpecialMoveAbility_Aoe.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpSpecialMoveAbility_Aoe : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpSpecialMoveAbility_Aoe();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
