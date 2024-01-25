// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "Utils/Enums.h"
#include "UpActivateEquipmentAbility.generated.h"

class AUpCharacter;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class UNREALPORTFOLIO_API UUpActivateEquipmentAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpActivateEquipmentAbility();

	FORCEINLINE EUpEquipmentSlot::Type GetEquipmentSlot() const { return EquipmentSlot; }

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(Transient)
	TEnumAsByte<EUpEquipmentSlot::Type> EquipmentSlot = EUpEquipmentSlot::Weapon1;
	UPROPERTY(Transient)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitGameplayEventTask;

	bool bActivate = false;

	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageInterrupted();
	
	FName GetMontageSectionName(const AUpCharacter* Character) const;
};
