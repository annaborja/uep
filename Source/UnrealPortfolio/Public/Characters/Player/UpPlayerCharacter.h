// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpPlayableCharacter.h"
#include "UpPlayerCharacter.generated.h"

class AUpPlayableNpc;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpPlayableCharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TArray<TSubclassOf<AUpPlayableNpc>> PlayableNpcClasses;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float SquadMemberSpawnLocationOffset_Forward = 300.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float SquadMemberSpawnLocationOffset_Right = 200.f;
};
