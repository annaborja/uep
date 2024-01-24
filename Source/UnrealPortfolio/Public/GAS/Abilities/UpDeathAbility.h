// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpDeathAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpDeathAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpDeathAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	virtual void OnMontageCompleted();
	
	static FName GetMontageSectionName();
};
