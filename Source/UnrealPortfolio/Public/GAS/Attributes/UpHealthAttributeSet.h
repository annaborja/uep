// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "UpHealthAttributeSet.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpHealthAttributeSet : public UUpAttributeSet
{
	GENERATED_BODY()

public:
	UUpHealthAttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UUpHealthAttributeSet, Health)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UUpHealthAttributeSet, MaxHealth)
};
