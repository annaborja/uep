// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealPortfolioGameModeBase.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUnrealPortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealPortfolioGameModeBase();

	FORCEINLINE float GetWalkSpeedModifierMultiplier() const { return WalkSpeedModifierMultiplier; }

private:
	UPROPERTY(EditAnywhere, Category="UP Params")
	float WalkSpeedModifierMultiplier = 1.f;
};
