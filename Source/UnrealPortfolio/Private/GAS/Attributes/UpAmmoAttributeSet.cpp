// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpAmmoAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "UpGameInstance.h"
#include "Characters/UpPlayableCharacter.h"
#include "GAS/UpGasDataAsset.h"
#include "Items/UpWeapon.h"
#include "Tags/GasTags.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

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

	if (Data.EvaluatedData.Attribute == GetAmmoReserveAttribute())
	{
		const auto OldAmmoReserve = GetAmmoReserve();
		const auto Max = GetMaxAmmo() - GetMagazineFill();
		const auto Excess = FMath::Max(0.f, OldAmmoReserve - Max);
		
		SetAmmoReserve(FMath::Clamp(GetAmmoReserve(), 0.f, Max));
		
		UE_LOG(LogTemp, Warning, TEXT("Old AmmoReserve: %g, Max: %g, Excess: %g, Magnitude: %g, New AmmoReserve: %g"),
			OldAmmoReserve, Max, Excess, Data.EvaluatedData.Magnitude, GetAmmoReserve())

		if (const auto Weapon = Cast<AUpWeapon>(Data.Target.GetAvatarActor()))
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(Weapon))
			{
				if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
				{
					if (const auto Delta = Data.EvaluatedData.Magnitude - Excess;
						Delta > 0.f && Data.EffectSpec.Def.GetClass() == GasDataAsset->GetEffectClass_AmmoGrant())
					{
						if (const auto CustomHud = UUpBlueprintFunctionLibrary::GetCustomHud(Weapon))
						{
							CustomHud->BroadcastNotification(FUpNotificationData(FText::FromString(
								FString::Printf(TEXT("Gained <RichText.Bold>%d</> ammo for <RichText.Bold>%s</>"),
									FMath::FloorToInt(Delta), *Weapon->GetInGameName().ToString()))));
						}
					}
				}
			}
		}
	} else if (Data.EvaluatedData.Attribute == GetMagazineFillAttribute())
	{
		SetMagazineFill(FMath::Clamp(GetMagazineFill(), 0.f, GetMagazineCapacity()));
	}

	if (const auto AvatarActor = Data.Target.GetAvatarActor())
	{
		if (const auto Character = Cast<ACharacter>(AvatarActor->GetAttachParentActor()); Character && !Character->IsPlayerControlled())
		{
			// Make sure NPCs never run out of ammo.
			SetAmmoReserve(GetMaxAmmo() - GetMagazineFill());
		}
	}
}
