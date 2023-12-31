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
	UUpGameplayAbility();
	
	virtual FGameplayTag GetTagId() const override { return TagId; }

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Debug")
	bool bDebug = false;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
};
