// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpAmmoAttributeSet.h"

#include "Tags/AttributeTags.h"

UUpAmmoAttributeSet::UUpAmmoAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Ammo_MagazineFill, GetMagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_MagazineCapacity, GetMagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_AmmoReserve, GetMagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_MaxAmmo, GetMagazineFillAttribute);
}

void UUpAmmoAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
