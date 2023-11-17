// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UpAbilitySystemComponent.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void Init(AActor* InOwnerActor, AActor* InAvatarActor);

	void GrantAbility(const TSubclassOf<UGameplayAbility> AbilityClass);
	bool TryActivateAbilityByTag(const FGameplayTag& Tag);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayEffect>> InitAttributesEffectClasses;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilityClasses;
};
