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
