// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpSpecialMoveAbility_Pull.generated.h"

class UAbilityTask_WaitGameplayEvent;

UCLASS()
class UNREALPORTFOLIO_API UUpSpecialMoveAbility_Pull : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpSpecialMoveAbility_Pull();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;
	
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageCompleted();
};
