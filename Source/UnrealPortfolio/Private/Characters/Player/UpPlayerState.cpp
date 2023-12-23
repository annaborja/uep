// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerState.h"

#include "GAS/UpAbilitySystemComponent.h"
#include "GAS/Attributes/UpHealthAttributeSet.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"

AUpPlayerState::AUpPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	HealthAttributeSet = CreateDefaultSubobject<UUpHealthAttributeSet>(TEXT("HealthAttributeSet"));
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));
}

TArray<UUpAttributeSet*> AUpPlayerState::GetAttributeSets() const
{
	return TArray<UUpAttributeSet*> { HealthAttributeSet, PrimaryAttributeSet };
}
