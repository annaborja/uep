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

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_Success;

	UFUNCTION()
	virtual void OnMontageCompleted();
	UFUNCTION()
	virtual void OnMontageInterrupted();

	FName GetReloadsMontageSectionName() const;
};
