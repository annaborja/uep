// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpSpecialMoveAbility_Aoe.generated.h"

class UNiagaraSystem;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class UNREALPORTFOLIO_API UUpSpecialMoveAbility_Aoe : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpSpecialMoveAbility_Aoe();

	UNiagaraSystem* GetVfx_Impact() const { return Vfx_Impact; }

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UNiagaraSystem> Vfx_Impact;
	
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;
	
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageCompleted();
};
