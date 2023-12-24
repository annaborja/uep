// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

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

UENUM()
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
