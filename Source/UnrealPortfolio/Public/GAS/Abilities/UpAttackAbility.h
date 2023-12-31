// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpAttackAbility.generated.h"

class UAbilityTask_Repeat;

UCLASS()
class UNREALPORTFOLIO_API UUpAttackAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float RepeatInterval = -1.f;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
		
	UFUNCTION()
	virtual void HandleRepeatAction(int32 ActionNumber) {}

	void TriggerDamage(AActor* TargetActor) const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_Repeat> RepeatTask;
	
	TScriptDelegate<FWeakObjectPtr> OnRepeatDelegate;
};
