// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "UpPrimaryAttributeSet.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpPrimaryAttributeSet : public UUpAttributeSet
{
	GENERATED_BODY()

public:
	UUpPrimaryAttributeSet();

private:
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Creativity;
	ATTRIBUTE_ACCESSORS(UUpPrimaryAttributeSet, Creativity)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UUpPrimaryAttributeSet, Dexterity)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UUpPrimaryAttributeSet, Intelligence)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UUpPrimaryAttributeSet, Resilience)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UUpPrimaryAttributeSet, Speed)
	
	UPROPERTY(VisibleAnywhere)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UUpPrimaryAttributeSet, Strength)
};
