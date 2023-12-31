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
	FORCEINLINE TSubclassOf<UGameplayEffect> GetAmmoGrantEffectClass() const { return AmmoGrantEffectClass; }
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetGrantedAbilityClasses_Character() const { return GrantedAbilityClasses_Character; }
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetGrantedAbilityClasses_Common() const { return GrantedAbilityClasses_Common; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UGameplayEffect> AmmoGrantEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilityClasses_Character;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilityClasses_Common;
};
