// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/Components/UpPlayerCombatComponent.h"
#include "Interfaces/UpCombatable.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpPlayerCharacter.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpPlayableCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface,
	public IUpCombatable, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual UUpCombatComponent* GetCombatComponent() const override { return PlayerCombatComponent; }
	
	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;
};
