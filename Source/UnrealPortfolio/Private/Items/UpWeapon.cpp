// Copyright AB. All Rights Reserved.

#include "Items/UpWeapon.h"

#include "UpGameInstance.h"
#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpWeapon::AUpWeapon()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AmmoAttributeSet = CreateDefaultSubobject<UUpAmmoAttributeSet>(TEXT("AmmoAttributeSet"));
}

void AUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		WeaponData = GameInstance->GetWeaponData(TagId);
	}

	check(WeaponData.IsValid())
	
	check(InitAmmoCapacityAttributesEffectClass);
	check(InitAmmoAmountAttributesEffectClass);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->Init(this, this,
			TArray { InitAmmoCapacityAttributesEffectClass, InitAmmoAmountAttributesEffectClass },
			TArray<TSubclassOf<UGameplayAbility>> {});
	}
}
