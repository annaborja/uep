// Copyright AB. All Rights Reserved.

#include "Items/UpWeapon.h"

#include "GameplayEffectCustomApplicationRequirement.h"
#include "UpGameInstance.h"
#include "Characters/UpPlayableCharacter.h"
#include "GAS/UpGasDataAsset.h"
#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "Items/UpAmmo.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpWeapon::AUpWeapon()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AmmoAttributeSet = CreateDefaultSubobject<UUpAmmoAttributeSet>(TEXT("AmmoAttributeSet"));
}

FUpInteractionData AUpWeapon::GetInteractionData(const AController* Controller)
{
	FUpInteractionData InteractionData;
	InteractionData.Interactable = nullptr;

	if (const auto CustomPlayerController = Cast<AUpPlayerController>(Controller))
	{
		if (const auto PossessedCharacter = CustomPlayerController->GetPossessedCharacter())
		{
			InteractionData.Interactable = this;
		
			const auto& Equipment = PossessedCharacter->GetEquipment();
			const auto ActiveWeaponSlot = Equipment.GetPotentialActiveWeaponSlot();
		
			const auto ActiveWeapon = Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(ActiveWeaponSlot).ItemInstance.ItemActor);
			const auto InactiveWeapon = Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(
				ActiveWeaponSlot == EUpEquipmentSlot::Weapon1 ? EUpEquipmentSlot::Weapon2 : EUpEquipmentSlot::Weapon1).ItemInstance.ItemActor);
		
			const auto PrimaryWeapon = Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon1).ItemInstance.ItemActor);

			if (ActiveWeapon && ActiveWeapon->IsAmmoCompatible(this))
			{
				InteractionData.InteractionPromptText = FText::FromString(
					FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *ActiveWeapon->GetInGameName().ToString()));
			
				if (!UUpBlueprintFunctionLibrary::CanGrantAmmo(ActiveWeapon))
				{
					InteractionData.InteractionPromptSubText = FText::FromString(TEXT("(Ammo Full)"));
				}
			} else if (InactiveWeapon && InactiveWeapon->IsAmmoCompatible(this))
			{
				InteractionData.InteractionPromptText = FText::FromString(
					FString::Printf(TEXT("Ammo for <RichText.Bold>%s</>"), *InactiveWeapon->GetInGameName().ToString()));

				if (!UUpBlueprintFunctionLibrary::CanGrantAmmo(InactiveWeapon))
				{
					InteractionData.InteractionPromptSubText = FText::FromString(TEXT("(Ammo Full)"));
				}
			} else if (!PrimaryWeapon || PrimaryWeapon->GetInGameWeaponType() != GetInGameWeaponType())
			{
				InteractionData.InteractionPromptText = FText::FromString(
					FString::Printf(TEXT("Equip <RichText.Bold>%s</>"), *GetInGameName().ToString()));
			
				if (const auto SecondaryWeapon = Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon2).ItemInstance.ItemActor)
				{
					InteractionData.InteractionPromptSubText = FText::FromString(
						FString::Printf(TEXT("(Swap for <RichText.Bold>%s</>)"), *SecondaryWeapon->GetInGameName().ToString()));
				}
			} else
			{
				InteractionData.Interactable = nullptr;
			}
		}
	}
		
	return InteractionData;
}

bool AUpWeapon::Interact(AController* Controller)
{
	if (const auto CustomPlayerController = Cast<AUpPlayerController>(Controller))
	{
		if (const auto PossessedCharacter = CustomPlayerController->GetPossessedCharacter())
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
			{
				if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
				{
					if (const auto AmmoGrantEffectClass = GasDataAsset->GetEffectClass_AmmoGrant()) {
						const auto& Equipment = PossessedCharacter->GetEquipment();
						const auto ActiveWeaponSlot = Equipment.GetPotentialActiveWeaponSlot();
		
						const auto ActiveWeapon = Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(ActiveWeaponSlot).ItemInstance.ItemActor);
						const auto InactiveWeapon = Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(
							ActiveWeaponSlot == EUpEquipmentSlot::Weapon1 ? EUpEquipmentSlot::Weapon2 : EUpEquipmentSlot::Weapon1).ItemInstance.ItemActor);
		
						const auto PrimaryWeapon = Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon1).ItemInstance.ItemActor);

						if (ActiveWeapon && ActiveWeapon->IsAmmoCompatible(this))
						{
							if (const auto TargetAbilitySystemComponent = ActiveWeapon->GetAbilitySystemComponent())
							{
								auto EffectContext = TargetAbilitySystemComponent->MakeEffectContext();
								EffectContext.AddSourceObject(this);
							
								if (TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
									*TargetAbilitySystemComponent->MakeOutgoingSpec(AmmoGrantEffectClass, 1.f, EffectContext).Data.Get()).WasSuccessfullyApplied())
								{
									OnAmmoGrantSuccess();
								}
							}
						} else if (InactiveWeapon && InactiveWeapon->IsAmmoCompatible(this))
						{
							if (const auto TargetAbilitySystemComponent = InactiveWeapon->GetAbilitySystemComponent())
							{
								auto EffectContext = TargetAbilitySystemComponent->MakeEffectContext();
								EffectContext.AddSourceObject(this);
							
								if (TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
									*TargetAbilitySystemComponent->MakeOutgoingSpec(AmmoGrantEffectClass, 1.f, EffectContext).Data.Get()).WasSuccessfullyApplied())
								{
									OnAmmoGrantSuccess();
								}
							}
						} else if (!PrimaryWeapon || PrimaryWeapon->GetInGameWeaponType() != GetInGameWeaponType())
						{
							PossessedCharacter->EquipItem(this, EUpEquipmentSlot::Weapon2);
						}
					}
				}
			}
		}
	}

	return true;
}

EUpCharacterPosture::Type AUpWeapon::GetCharacterPosture() const
{
	if (IsPistolType()) return EUpCharacterPosture::ArmedPistol;
	if (IsRifleType()) return EUpCharacterPosture::ArmedRifle;

	return Super::GetCharacterPosture();
}

float AUpWeapon::CalculateDamage() const
{
	return 25.f;
}

FVector2D AUpWeapon::CalculateRecoil() const
{
	if (TagId.MatchesTag(TAG_Item_Weapon_Pistol))
	{
		return FVector2D(0.f, 0.8f);
	} else if (TagId.MatchesTag(TAG_Item_Weapon_AssaultRifle))
	{
		return FVector2D(0.2f, 0.2f);
	} else
	{
		return FVector2D(1.f, 0.f);
	}
}

float AUpWeapon::GetRecoilCenterTime() const
{
	return 0.5f;
}

float AUpWeapon::GetAmmoGrantAmount() const
{
	return FMath::RandRange(FMath::CeilToInt(GetMagazineCapacity() * 0.5f), FMath::CeilToInt(GetMaxAmmo() * 0.5f));
}

float AUpWeapon::GetMagazineCapacity() const
{
	return WeaponData.MagazineCapacity;
}

float AUpWeapon::GetMaxAmmo() const
{
	return WeaponData.MaxAmmo;
}

float AUpWeapon::GetBurstInterval() const
{
	return WeaponData.BurstInterval;
}

float AUpWeapon::GetBurstShotInterval() const
{
	return WeaponData.BurstShotInterval;
}

int8 AUpWeapon::GetBurstSize() const
{
	return WeaponData.BurstSize;
}

float AUpWeapon::GetRange() const
{
	return WeaponData.Range;
}

uint8 AUpWeapon::GetProjectilesPerShot() const
{
	if (TagId.MatchesTag(TAG_Item_Weapon_Shotgun)) return 8;

	return 1;
}

float AUpWeapon::GetSpreadAngleInDegrees() const
{
	if (TagId.MatchesTag(TAG_Item_Weapon_Shotgun)) return 8.f;
	
	return 3.f;
}

FString AUpWeapon::GetWeaponTypeNameSectionString() const
{
	if (IsPistolType()) return NAME_STRING_PISTOL_TYPE;
	if (IsRifleType()) return NAME_STRING_RIFLE_TYPE;

	return TEXT("");
}

EUpInGameWeaponType::Type AUpWeapon::GetInGameWeaponType() const
{
	if (IsRifleType()) return EUpInGameWeaponType::Rifle;

	return EUpInGameWeaponType::Pistol;
}

bool AUpWeapon::IsAmmoCompatible(const AUpWeapon* OtherWeapon) const
{
	const auto& OtherWeaponTagId = OtherWeapon->GetTagId();
	
	TArray<FString> OtherWeaponTagIdSegments;
	OtherWeaponTagId.ToString().ParseIntoArray(OtherWeaponTagIdSegments, TEXT("."));

	if (TagId.MatchesTag(FGameplayTag::RequestGameplayTag(FName(FString::Printf(TEXT("%s.%s.%s.Primary"),
		*OtherWeaponTagIdSegments[0], *OtherWeaponTagIdSegments[1], *OtherWeaponTagIdSegments[2])))))
	{
		return true;
	}

	return TagId.MatchesTagExact(OtherWeaponTagId);
}

bool AUpWeapon::IsPistolType() const
{
	FGameplayTagContainer Tags;
	Tags.AddTag(TAG_Item_Weapon_Pistol);
	Tags.AddTag(TAG_Item_Weapon_Revolver);
	
	return TagId.MatchesAny(Tags);
}

bool AUpWeapon::IsRifleType() const
{
	FGameplayTagContainer Tags;
    Tags.AddTag(TAG_Item_Weapon_AssaultRifle);
    Tags.AddTag(TAG_Item_Weapon_Shotgun);
    Tags.AddTag(TAG_Item_Weapon_SniperRifle);
    Tags.AddTag(TAG_Item_Weapon_SubmachineGun);
    
    return TagId.MatchesAny(Tags);
}

void AUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(TagId.MatchesTag(TAG_Item_Weapon));
	
	check(Sfx_Equip);

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		WeaponData = GameInstance->GetWeaponData(TagId);
	}

	check(WeaponData.IsValid())
	
	check(EffectClass_InitAmmo);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->Init(this, this,
			TArray { EffectClass_InitAmmo }, TArray<TSubclassOf<UGameplayAbility>> {});
	}
}

void AUpWeapon::OnAmmoGrantSuccess()
{
	if (const auto Sound = WeaponData.Sfx_AmmoGrant)
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}

	Destroy();
}
