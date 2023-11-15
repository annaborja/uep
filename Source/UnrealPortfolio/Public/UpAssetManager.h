// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UpAssetManager.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UUpAssetManager& Get();
	
	virtual void StartInitialLoading() override;
};
