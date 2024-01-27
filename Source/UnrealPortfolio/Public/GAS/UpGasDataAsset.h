// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpGasDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class UNREALPORTFOLIO_API UUpGasDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TSubclassOf<UGameplayEffect> GetEffectClass_AmmoGrant() const { return EffectClass_AmmoGrant; }
	FORCEINLINE TSubclassOf<UGameplayEffect> GetEffectClass_BusyState() const { return EffectClass_BusyState; }
	FORCEINLINE TSubclassOf<UGameplayEffect> GetEffectClass_Damage() const { return EffectClass_Damage; }
	FORCEINLINE TSubclassOf<UGameplayEffect> GetEffectClass_StaminaRegenCooldown() const { return EffectClass_StaminaRegenCooldown; }
	
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetGrantedAbilityClasses_Common() const { return GrantedAbilityClasses_Common; }
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetGrantedAbilityClasses_Character() const { return GrantedAbilityClasses_Character; }
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetGrantedAbilityClasses_PlayableCharacter() const { return GrantedAbilityClasses_PlayableCharacter; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_AmmoGrant;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_BusyState;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_Damage;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> EffectClass_StaminaRegenCooldown;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilityClasses_Common;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilityClasses_Character;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilityClasses_PlayableCharacter;
};
