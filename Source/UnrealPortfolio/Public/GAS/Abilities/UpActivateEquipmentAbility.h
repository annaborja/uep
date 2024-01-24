// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "Utils/Enums.h"
#include "UpActivateEquipmentAbility.generated.h"

class AUpCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpActivateEquipmentAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpActivateEquipmentAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(Transient)
	TEnumAsByte<EUpEquipmentSlot::Type> EquipmentSlot = EUpEquipmentSlot::Weapon1;
	
	UFUNCTION()
	virtual void OnMontageCompleted();
	UFUNCTION()
	virtual void OnMontageInterrupted();
	
	FName GetMontageSectionName(const AUpCharacter* Character) const;
};
