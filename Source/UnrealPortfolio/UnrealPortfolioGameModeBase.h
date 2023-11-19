// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealPortfolioGameModeBase.generated.h"

class UDataTable;
class UDialogueVoice;
class UUpGasDataAsset;

UCLASS()
class UNREALPORTFOLIO_API AUnrealPortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void GetPlayerCharacterTags(FGameplayTagContainer& OutCharacterTags) const;
	bool AddPlayerCharacterTag(const FGameplayTag& Tag);
	bool RemovePlayerCharacterTag(const FGameplayTag& Tag);
	
	void GetNpcCharacterTags(const FGameplayTag& NpcTagId, FGameplayTagContainer& OutCharacterTags);
	bool AddNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag);
	bool RemoveNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag);

	FUpReputationData GetPlayerReputationData(const FGameplayTag& TagId);
	bool UpdatePlayerReputation_Affection(const FGameplayTag& TagId, const int8 Delta);
	bool UpdatePlayerReputation_Esteem(const FGameplayTag& TagId, const int8 Delta);
	
	FORCEINLINE UUpGasDataAsset* GetGasDataAsset() const { return GasDataAsset; }
	FORCEINLINE UDataTable* GetNpcDataTable() const { return NpcDataTable; }
	FORCEINLINE UDialogueVoice* GetPlayerDialogueVoice() const { return PlayerDialogueVoice; }
	FORCEINLINE FUpReputationData GetPlayerWorldReputationData() const { return PlayerWorldReputationData; }
	FORCEINLINE TMap<FGameplayTag, FUpReputationData> GetPlayerNpcReputationDataMap() const { return PlayerNpcReputationDataMap; }
	FORCEINLINE bool ShouldDebugTagSpecGrant() const { return bDebugTagSpecGrant; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UUpGasDataAsset> GasDataAsset;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="UpNpcData"))
	TObjectPtr<UDataTable> NpcDataTable;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDialogueVoice> PlayerDialogueVoice;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Debug")
	bool bDebugTagSpecGrant = false;

	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime")
	FGameplayTagContainer PlayerCharacterTags;
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime")
	FUpReputationData PlayerWorldReputationData;
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime")
	TMap<FGameplayTag, FUpReputationData> PlayerNpcReputationDataMap;
	
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime")
	TMap<FGameplayTag, FGameplayTagContainer> NpcCharacterTagsMap;
};
