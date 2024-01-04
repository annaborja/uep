// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "UpPlayerState.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AUpPlayerState();
	
	void GetSquadMemberTags(FGameplayTagContainer& OutTags) const;

private:
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTagContainer SquadMemberTags;
};
