// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerState.h"

#include "GAS/UpAbilitySystemComponent.h"

AUpPlayerState::AUpPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

TArray<UUpAttributeSet*> AUpPlayerState::GetAttributeSets() const
{
	return TArray<UUpAttributeSet*> {};
}
