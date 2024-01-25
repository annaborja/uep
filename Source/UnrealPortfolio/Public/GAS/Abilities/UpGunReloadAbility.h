// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpGunReloadAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpGunReloadAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpGunReloadAbility();

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass;

	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageInterrupted();
};
