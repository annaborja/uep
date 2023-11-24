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
	void Init(AActor* InOwnerActor, AActor* InAvatarActor,
		const TArray<TSubclassOf<UGameplayEffect>>& InitAttributesEffectClasses, const TArray<TSubclassOf<UGameplayAbility>>& GrantedAbilityClasses);

	void GrantAbility(const TSubclassOf<UGameplayAbility> AbilityClass);
	bool TryActivateAbilityByTag(const FGameplayTag& Tag);
};
