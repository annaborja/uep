// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/UpPlayableCharacter.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpPlayerCharacter.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpPlayableCharacter, public IGameplayTagAssetInterface, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;
};
