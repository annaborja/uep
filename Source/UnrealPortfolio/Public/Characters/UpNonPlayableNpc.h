// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "UpNonPlayableNpc.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpNonPlayableNpc : public AUpCharacter
{
	GENERATED_BODY()

public:
	explicit AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer);
};
