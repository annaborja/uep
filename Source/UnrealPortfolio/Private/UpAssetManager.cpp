// Copyright AB. All Rights Reserved.

#include "UpAssetManager.h"

#include "AbilitySystemGlobals.h"

UUpAssetManager& UUpAssetManager::Get()
{
	check(GEngine);
	
	return *CastChecked<UUpAssetManager>(GEngine->AssetManager);
}

void UUpAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
