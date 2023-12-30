// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpAmmoAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Tags/AttributeTags.h"

UUpAmmoAttributeSet::UUpAmmoAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Ammo_MagazineFill, GetMagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_MagazineCapacity, GetMagazineCapacityAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_AmmoReserve, GetAmmoReserveAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_MaxAmmo, GetMaxAmmoAttribute);
}

void UUpAmmoAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
