// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpGameplayAbility.h"
#include "UpGunFireAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpGunFireAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpGunFireAbility();

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_AmmoCost;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual float GetRepeatInterval() const override;
	virtual void HandleRepeatAction(int32 ActionNumber) override;

private:
	uint8 BurstShotCount = 0;

	void ResetBurstShotCount();
};
