// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpHitReactionAbility.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpHitReactionAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpHitReactionAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnMontageEnd();

	FName GetHitReactionsMontageSectionName(const FVector& Origin, const FHitResult& HitResult) const;
};
