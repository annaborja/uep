// Copyright AB. All Rights Reserved.

#include "Items/UpWeapon.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableCharacter.h"
#include "GAS/UpGasDataAsset.h"
#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpAmmo.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpWeapon::AUpWeapon()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AmmoAttributeSet = CreateDefaultSubobject<UUpAmmoAttributeSet>(TEXT("AmmoAttributeSet"));
}

FUpInteractionData AUpWeapon::GetInteractionData(const AUpPlayerController* PlayerController)
{
	FUpInteractionData InteractionData;
	InteractionData.Interactable = this;

	if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
	{
		const auto& Weapon1SlotData = PossessedCharacter->GetEquipment().GetEquipmentSlotData(EUpEquipmentSlot::Weapon1);
		const auto& Weapon2SlotData = PossessedCharacter->GetEquipment().GetEquipmentSlotData(EUpEquipmentSlot::Weapon2);
		const auto Weapon1 = Weapon1SlotData.ItemInstance.ItemActor;
		const auto Weapon2 = Weapon2SlotData.ItemInstance.ItemActor;

		const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
		const auto GasDataAsset = GameInstance ? GameInstance->GetGasDataAsset() : nullptr;
		const auto AmmoGrantEffectClass = GasDataAsset ? GasDataAsset->GetAmmoGrantEffectClass() : nullptr;

		if (!Weapon2)
		{
			// If the character has no weapon 2...
			// Then equip this weapon as weapon 2.
			InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Equip <RichText.Bold>%s</>"), *ItemData.Name.ToString()));
		} else if (!Weapon2->GetTagId().MatchesTagExact(TagId))
		{
			// If weapon 2 is not compatible with this weapon...
			// Then swap weapon 2 for this weapon.
			InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Equip <RichText.Bold>%s</>"), *ItemData.Name.ToString()));
			InteractionData.InteractionPromptSubText = FText::FromString(FString::Printf(TEXT("(Swap for <RichText.Bold>%s</>)"), *Weapon2->GetInGameName().ToString()));
		} else if (Weapon1 && Weapon1->GetTagId().MatchesTagExact(TagId))
		{
			// If both weapon 1 and weapon 2 are compatible with this weapon...
			// And if weapon 2 is active...
			if (Weapon2SlotData.bActivated)
			{
				if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon2SlotData))
				{
					// If weapon 2 has room for ammo...
					// Then add ammo for weapon 2.
					InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon2->GetInGameName().ToString()));
				} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon1SlotData))
				{
					// If weapon 1 has room for ammo...
					// Then add ammo for weapon 1.
					InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon1->GetInGameName().ToString()));
				} else
				{
					// Otherwise, indicate that weapon 2 is full.
					InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon2->GetInGameName().ToString()));
					InteractionData.InteractionPromptSubText = FText::FromString(TEXT("(Ammo Full)"));
				}
			} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon1SlotData))
			{
				// If weapon 1 has room for ammo...
				// Then add ammo for weapon 1.
				InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon1->GetInGameName().ToString()));
			} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon2SlotData))
			{
				// If weapon 2 has room for ammo...
				// Then add ammo for weapon 2.
				InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon2->GetInGameName().ToString()));
			} else
			{
				// Otherwise, indicate that weapon 1 is full.
				InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon1->GetInGameName().ToString()));
				InteractionData.InteractionPromptSubText = FText::FromString(TEXT("(Ammo Full)"));
			}
		} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon2SlotData))
		{
			// If weapon 2 has room for ammo...
			// Then add ammo for weapon 2.
			InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon2->GetInGameName().ToString()));
		}  else
		{
			// Otherwise, indicate that weapon 2's ammo is full.
			InteractionData.InteractionPromptText = FText::FromString(FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *Weapon2->GetInGameName().ToString()));
			InteractionData.InteractionPromptSubText = FText::FromString(TEXT("(Ammo Full)"));
		}
	}
	
	return InteractionData;
}

void AUpWeapon::Interact(AUpPlayerController* PlayerController)
{
	if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
	{
		const auto& Weapon1SlotData = PossessedCharacter->GetEquipment().GetEquipmentSlotData(EUpEquipmentSlot::Weapon1);
		const auto& Weapon2SlotData = PossessedCharacter->GetEquipment().GetEquipmentSlotData(EUpEquipmentSlot::Weapon2);
		const auto Weapon1 = Weapon1SlotData.ItemInstance.ItemActor;
		const auto Weapon2 = Weapon2SlotData.ItemInstance.ItemActor;

		const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
		const auto GasDataAsset = GameInstance ? GameInstance->GetGasDataAsset() : nullptr;
		const auto AmmoGrantEffectClass = GasDataAsset ? GasDataAsset->GetAmmoGrantEffectClass() : nullptr;

		if (!Weapon2 || !Weapon2->GetTagId().MatchesTagExact(TagId))
		{
			// If the character has no weapon 2...
			// Then equip this weapon as weapon 2.
			PossessedCharacter->EquipItem(this, EUpEquipmentSlot::Weapon2);
		} else if (Weapon1 && Weapon1->GetTagId().MatchesTagExact(TagId))
		{
			// If both weapon 1 and weapon 2 are compatible with this weapon...
			// And if weapon 2 is active...
			if (Weapon2SlotData.bActivated)
			{
				if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon2SlotData))
				{
					// If weapon 2 has room for ammo...
					// Then add ammo for weapon 2.
					UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
				} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon1SlotData))
				{
					// If weapon 1 has room for ammo...
					// Then add ammo for weapon 1.
					UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
				} else
				{
					// Otherwise, indicate that weapon 2 is full.
					UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
					UE_LOG(LogTemp, Warning, TEXT("(Ammo Full)"));
				}
			} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon1SlotData))
			{
				// If weapon 1 has room for ammo...
				// Then add ammo for weapon 1.
				UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
			} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon2SlotData))
			{
				// If weapon 2 has room for ammo...
				// Then add ammo for weapon 2.
				UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
			} else
			{
				// Otherwise, indicate that weapon 1 is full.
				UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
				UE_LOG(LogTemp, Warning, TEXT("(Ammo Full)"));
			}
		} else if (AmmoGrantEffectClass && AUpAmmo::TryGrantAmmoForWeaponSlot(AmmoGrantEffectClass, TagId, 0, Weapon2SlotData))
		{
			// If weapon 2 has room for ammo...
			// Then add ammo for weapon 2.
			UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
		}  else
		{
			// Otherwise, indicate that weapon 2's ammo is full.
			UE_LOG(LogTemp, Warning, TEXT("Ammo for <RichText.Bold>s</>"));
			UE_LOG(LogTemp, Warning, TEXT("(Ammo Full)"));
		}
	}
}

float AUpWeapon::CalculateDamage() const
{
	return 25.f;
}

FName AUpWeapon::GetMontageSectionName() const
{
	TArray<FString> TagSegments;
	TagId.ToString().ParseIntoArray(TagSegments, TEXT("."));

	return FName(TagSegments[2]);
}

float AUpWeapon::GetBurstShotInterval() const
{
	return WeaponData.BurstShotInterval;
}

int32 AUpWeapon::GetBurstSize() const
{
	return WeaponData.BurstSize;
}

float AUpWeapon::GetRange() const
{
	return WeaponData.Range;
}

void AUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(TagId.MatchesTag(TAG_Item_Weapon));

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
