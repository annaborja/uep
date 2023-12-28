// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpAmmoAttributeSet.h"

#include "Tags/AttributeTags.h"

UUpAmmoAttributeSet::UUpAmmoAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon1MagazineFill, GetWeapon1MagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon1MagazineCapacity, GetWeapon1MagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon1AmmoReserve, GetWeapon1MagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon1MaxAmmo, GetWeapon1MagazineFillAttribute);
	
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon2MagazineFill, GetWeapon2MagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon2MagazineCapacity, GetWeapon2MagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon2AmmoReserve, GetWeapon2MagazineFillAttribute);
	TagAttributeMap.Add(TAG_Attribute_Ammo_Weapon2MaxAmmo, GetWeapon2MagazineFillAttribute);
}

void UUpAmmoAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
