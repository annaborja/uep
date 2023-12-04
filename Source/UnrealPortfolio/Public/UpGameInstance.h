// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Engine/GameInstance.h"
#include "UpGameInstance.generated.h"

class UDataTable;
class UDialogueVoice;
class UUpGasDataAsset;

UCLASS()
class UNREALPORTFOLIO_API UUpGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	void GetPlayerCharacterTags(FGameplayTagContainer& OutTags) const;
	bool AddPlayerCharacterTag(const FGameplayTag& Tag);
	bool RemovePlayerCharacterTag(const FGameplayTag& Tag);
	
	void GetNpcCharacterTags(const FGameplayTag& NpcTagId, FGameplayTagContainer& OutTags);
	bool AddNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag);
	bool RemoveNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag);

	void GetPartyMemberTags(FGameplayTagContainer& OutTags) const;
	void AddPartyMember(const FGameplayTag& NpcTagId);
	void RemovePartyMember(const FGameplayTag& NpcTagId);

	FUpReputationData GetPlayerReputationData(const FGameplayTag& TagId);
	bool UpdatePlayerReputation_Affection(const FGameplayTag& TagId, const int8 Delta);
	bool UpdatePlayerReputation_Esteem(const FGameplayTag& TagId, const int8 Delta);
	
	FORCEINLINE UUpGasDataAsset* GetGasDataAsset() const { return GasDataAsset; }
	FORCEINLINE UDataTable* GetNpcDataTable() const { return NpcDataTable; }
	
	FORCEINLINE UDialogueVoice* GetPlayerDialogueVoice() const { return PlayerDialogueVoice; }
	FORCEINLINE FUpReputationData GetPlayerWorldReputationData() const { return PlayerWorldReputationData; }
	FORCEINLINE TMap<FGameplayTag, FUpReputationData> GetPlayerNpcReputationDataMap() const { return PlayerNpcReputationDataMap; }
	
	FORCEINLINE bool ShouldDebugTagSpecGrant() const { return bDebugTagSpecGrant; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UUpGasDataAsset> GasDataAsset;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpNpcData"))
	TObjectPtr<UDataTable> NpcDataTable;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDialogueVoice> PlayerDialogueVoice;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugTagSpecGrant = false;
	
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Tags")
	FGameplayTagContainer PlayerCharacterTags;
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Tags")
	TMap<FGameplayTag, FGameplayTagContainer> NpcCharacterTagsMap;
	
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Party")
	FGameplayTagContainer PartyMemberTags;
	
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Reputation")
	FUpReputationData PlayerWorldReputationData;
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Reputation")
	TMap<FGameplayTag, FUpReputationData> PlayerNpcReputationDataMap;
};
