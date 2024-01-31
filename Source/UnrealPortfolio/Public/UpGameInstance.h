// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/Player/Components/UpPlayerPartyComponent.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "Engine/GameInstance.h"
#include "Utils/Structs.h"
#include "UpGameInstance.generated.h"

class AUpCharacter;
class AUpLevelScriptActor;
struct FUpCharacterData;
class UDataTable;
class UDialogueVoice;
class UUpGasDataAsset;

UCLASS()
class UNREALPORTFOLIO_API UUpGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	FUpCharacterData GetCharacterData(const FGameplayTag& NpcTagId);
	
	void GetPlayerCharacterTags(FGameplayTagContainer& OutTags) const;
	bool AddPlayerCharacterTag(const FGameplayTag& Tag);
	bool RemovePlayerCharacterTag(const FGameplayTag& Tag);
	
	void GetNpcCharacterTags(const FGameplayTag& NpcTagId, FGameplayTagContainer& OutTags);
	bool AddNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag);
	bool RemoveNpcCharacterTag(const FGameplayTag& NpcTagId, const FGameplayTag& Tag);

	FUpPartyMembershipSpec GetPartyMembershipSpec(const FGameplayTag& TagId) const;
	TMap<FGameplayTag, FUpPartyMembershipSpec> GetPartyMembershipSpecMap() const { return PartyMembershipSpecMap; }
	
	FUpReputationData GetPlayerReputationData(const FGameplayTag& TagId);
	bool UpdatePlayerReputation_Affection(const FGameplayTag& TagId, const int8 Delta);
	bool UpdatePlayerReputation_Esteem(const FGameplayTag& TagId, const int8 Delta);

	FUpItemData GetItemData(const FGameplayTag& ItemTagId);
	FUpInventory GetNpcInventory(const FGameplayTag& NpcTagId);
	FUpWeaponData GetWeaponData(const FGameplayTag& WeaponTagId);

	void ApplyBusyState(AUpCharacter* Character) const;
	void RemoveBusyState(AUpCharacter* Character) const;

	void SetLevelScriptActor(AUpLevelScriptActor* InLevelScriptActor) { LevelScriptActor = InLevelScriptActor; }
	FORCEINLINE AUpLevelScriptActor* GetLevelScriptActor() const { return LevelScriptActor; }
	
	FORCEINLINE UDataTable* GetCharacterDataTable() const { return CharacterDataTable; }
	FORCEINLINE UUpGasDataAsset* GetGasDataAsset() const { return GasDataAsset; }
	
	FORCEINLINE UDialogueVoice* GetPlayerDialogueVoice() const { return PlayerDialogueVoice; }
	FORCEINLINE FUpReputationData GetPlayerWorldReputationData() const { return PlayerWorldReputationData; }
	FORCEINLINE TMap<FGameplayTag, FUpReputationData> GetPlayerNpcReputationDataMap() const { return PlayerNpcReputationDataMap; }
	
	FORCEINLINE bool ShouldDebugTagSpecGrant() const { return bDebugTagSpecGrant; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UUpGasDataAsset> GasDataAsset;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDialogueVoice> PlayerDialogueVoice;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|DataTables", meta=(RowType="/Script/UnrealPortfolio.UpCharacterData"))
	TObjectPtr<UDataTable> CharacterDataTable;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|DataTables", meta=(RowType="/Script/UnrealPortfolio.UpItemData"))
	TObjectPtr<UDataTable> ItemDataTable;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|DataTables", meta=(RowType="/Script/UnrealPortfolio.UpWeaponData"))
	TObjectPtr<UDataTable> WeaponDataTable;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugTagSpecGrant = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	TMap<FGameplayTag, FUpPartyMembershipSpec> PartyMembershipSpecMap;
	
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Tags")
	FGameplayTagContainer PlayerCharacterTags;
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Tags")
	TMap<FGameplayTag, FGameplayTagContainer> NpcCharacterTagsMap;
	
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Reputation")
	FUpReputationData PlayerWorldReputationData;
	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Reputation")
	TMap<FGameplayTag, FUpReputationData> PlayerNpcReputationDataMap;

	// TODO(P0): Load from saved data.
	UPROPERTY(SaveGame, EditAnywhere, Category="UP Runtime|Inventory")
	TMap<FGameplayTag, FUpInventory> NpcInventoryMap;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category="UP Runtime")
	TObjectPtr<AUpLevelScriptActor> LevelScriptActor;
	
	UPROPERTY(Transient)
	TArray<FUpCharacterData> AllCharacterData;
	UPROPERTY(Transient)
	TArray<FUpItemData> AllItemData;
	UPROPERTY(Transient)
	TArray<FUpWeaponData> AllWeaponData;
};
