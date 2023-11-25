// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealPortfolioGameModeBase.h"

#include "Characters/Player/UpPlayerState.h"

AUnrealPortfolioGameModeBase::AUnrealPortfolioGameModeBase()
{
	PlayerStateClass = AUpPlayerState::StaticClass();
}
