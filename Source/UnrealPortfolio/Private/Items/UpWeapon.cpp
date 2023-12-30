// Copyright AB. All Rights Reserved.

#include "Items/UpWeapon.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"

AUpWeapon::AUpWeapon()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AmmoAttributeSet = CreateDefaultSubobject<UUpAmmoAttributeSet>(TEXT("AmmoAttributeSet"));
}

void AUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(InitAmmoCapacityAttributesEffectClass);
	check(InitAmmoAmountAttributesEffectClass);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->Init(this, this,
			TArray { InitAmmoCapacityAttributesEffectClass, InitAmmoAmountAttributesEffectClass },
			TArray<TSubclassOf<UGameplayAbility>> {});
	}
}
