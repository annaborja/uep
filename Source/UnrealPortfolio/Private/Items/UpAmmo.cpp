// Copyright AB. All Rights Reserved.

#include "Items/UpAmmo.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

FText AUpAmmo::GetInteractionPrompt() const
{
	if (const auto PlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if (const auto& WeaponTag = GetInteractionRelatedTag(PlayerController); WeaponTag.IsValid() && UUpInventoryComponent::IsWeaponTag(WeaponTag))
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
			{
				if (const auto& WeaponData = GameInstance->GetItemData(WeaponTag); WeaponData.IsValid())
				{
					return FText::FromString(FString::Printf(TEXT("%s for <RichText.Emphasis>%s</>"), *GetInGameName().ToString(), *WeaponData.Name.ToString()));
				}
			}
		}
	}
	
	return Super::GetInteractionPrompt();
}

int32 AUpAmmo::GetInteractionQuantity(const AUpPlayerController* PlayerController, const FGameplayTag& DynamicRelatedTag) const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto WeaponData = GameInstance->GetWeaponData(DynamicRelatedTag); WeaponData.IsValid())
		{
			return WeaponData.BaseMagazineCapacity;
		}
	}
	
	return Super::GetInteractionQuantity(PlayerController, DynamicRelatedTag);
}

FGameplayTag AUpAmmo::GetInteractionRelatedTag(const AUpPlayerController* PlayerController) const
{
	// If the ammo's weapon tag isn't explicitly set, set it to the character's active or primary weapon.
	if (!RelatedTag.IsValid())
	{
		if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
		{
			if (const auto& WeaponSlotData = PossessedCharacter->GetEquipment().GetPotentialActiveWeaponSlotData();
				WeaponSlotData.ItemInstance.ItemClass)
			{
				if (const auto DefaultObject = WeaponSlotData.ItemInstance.ItemClass.GetDefaultObject())
				{
					if (const auto& WeaponTag = DefaultObject->GetTagId(); WeaponTag.IsValid())
					{
						return WeaponTag;
					}
				}
			}
		}
	}
	
	return Super::GetInteractionRelatedTag(PlayerController);
}
