// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM()
namespace EUpAbilityGrantDuration
{
	enum Type : uint8
	{
		WhileEquipped,
		Permanent
	};
}

UENUM(BlueprintType)
namespace EUpCameraView
{
	enum Type : uint8
	{
		FirstPerson,
		FirstPerson_Debug,
		ThirdPerson,
		ThirdPerson_OverTheShoulder
	};
}

UENUM(BlueprintType)
namespace EUpCharacterPosture
{
	enum Type : uint8 {
		Casual,
		ArmedPistol,
		ArmedRevolver,
		ArmedRifle
	};
}

UENUM(BlueprintType)
namespace EUpEquipmentSlot
{
	enum Type : uint8
	{
		Weapon1,
		Weapon2,
		Item1,
		Item2,
		Helmet,
		Armor
	};
}

UENUM(BlueprintType)
namespace EUpItemCategory
{
	enum Type : uint8
	{
		Consumable,
		Permanent,
		Weapon,
		Wearable
	};
}
