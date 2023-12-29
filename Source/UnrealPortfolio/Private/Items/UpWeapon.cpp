// Copyright AB. All Rights Reserved.

#include "Items/UpWeapon.h"

#include "GAS/Attributes/UpAmmoAttributeSet.h"

AUpWeapon::AUpWeapon()
{
	AmmoAttributeSet = CreateDefaultSubobject<UUpAmmoAttributeSet>(TEXT("AmmoAttributeSet"));
}

void AUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(InitAmmoAttributesEffectClass);
}
