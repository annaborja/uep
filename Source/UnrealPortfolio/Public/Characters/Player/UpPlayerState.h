// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "Utils/Structs.h"
#include "UpPlayerState.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AUpPlayerState();
	
	void GetSquadMemberTags(FGameplayTagContainer& OutTags) const;
	void ProgressMission(const FGameplayTag& TagId);

	FORCEINLINE TMap<FGameplayTag, FUpMissionState> GetMissionMap() const { return MissionMap; }

private:
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTagContainer SquadMemberTags;
	UPROPERTY(EditAnywhere, Category="UP Params")
	TMap<FGameplayTag, FUpMissionState> MissionMap;
};
