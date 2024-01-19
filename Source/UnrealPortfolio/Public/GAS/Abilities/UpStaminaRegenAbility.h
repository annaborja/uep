// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpStaminaRegenAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpStaminaRegenAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpStaminaRegenAbility();
	
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void HandleRepeatAction(int32 ActionNumber) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	TSubclassOf<UGameplayEffect> EffectClass;
};
