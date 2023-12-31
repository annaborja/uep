// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "UpGameplayCueManager.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

	// Implemented on the advice of https://github.com/tranek/GASDocumentation?tab=readme-ov-file#485-gameplay-cue-manager.
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override { return false; }
};
