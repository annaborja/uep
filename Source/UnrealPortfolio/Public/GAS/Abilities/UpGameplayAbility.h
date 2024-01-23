// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UpGameplayAbility.generated.h"

class UAbilityTask_Repeat;

UCLASS()
class UNREALPORTFOLIO_API UUpGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UUpGameplayAbility();
	
	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Debug")
	bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float RepeatInterval = -1.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	int32 RepeatMaxActionCount = MAX_int32;
	
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_Repeat> RepeatTask;
	
	TScriptDelegate<FWeakObjectPtr> OnRepeatDelegate;
	
	UFUNCTION()
	virtual void HandleRepeatAction(int32 ActionNumber) {}

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual float GetRepeatInterval() const { return RepeatInterval; }
	virtual int32 GetRepeatMaxActionCount() const { return RepeatMaxActionCount; }
	
	void TriggerDamage(const FHitResult& HitResult) const;
};
