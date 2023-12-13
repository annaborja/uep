// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "UpPlayableCharacter.h"
#include "Engine/DataTable.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "Interfaces/UpCharacterEquippable.h"
#include "Interfaces/UpCombatable.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpNpcCharacter.generated.h"

class AUpAiController;
class UBehaviorTree;
class UDialogueVoice;
class USphereComponent;
class UUpDialogueComponent;
class UUpGameInstance;
class UUpPrimaryAttributeSet;
class UUpVitalAttributeSet;

USTRUCT(BlueprintType)
struct FUpNpcData : public FTableRowBase
{
	GENERATED_BODY()

	bool IsValid() const { return TagId.IsValid() && !Name.IsEmpty(); }

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_FullBody;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Head;
};

UCLASS()
class UNREALPORTFOLIO_API AUpNpcCharacter : public AUpPlayableCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface,
	public IUpCharacterEquippable, public IUpCombatable, public IUpInteractable, public IUpNameable, public IUpTagIdable, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpNpcCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual FUpCharacterEquipment GetCharacterEquipment() const override;
	virtual void ActivateEquipment(const EUpCharacterEquipmentSlot::Type EquipmentSlot, const FUpCharacterEquipmentSlotData& EquipmentSlotData) override;
	virtual void DeactivateEquipment(const EUpCharacterEquipmentSlot::Type EquipmentSlot, const FUpCharacterEquipmentSlotData& EquipmentSlotData) override;

	virtual UUpCombatComponent* GetCombatComponent() const override { return CombatComponent; }

	virtual bool CanInteract() const override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return NpcData.Name; }

	virtual FGameplayTag GetTagId() const override { return TagId; }

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	void JumpToLocation(const FVector& TargetLocation, const float Duration);
	bool Mantle() const;
	void ToggleSprint(const bool bSprint) const;

	FORCEINLINE FUpNpcData GetNpcData() const { return NpcData; }
	FORCEINLINE UUpPrimaryAttributeSet* GetPrimaryAttributeSet() const { return PrimaryAttributeSet; }
	FORCEINLINE UUpVitalAttributeSet* GetVitalAttributeSet() const { return VitalAttributeSet; }
	
	FORCEINLINE UTexture2D* GetImage_FullBody() const { return NpcData.Image_FullBody; }
	FORCEINLINE UTexture2D* GetImage_Head() const { return NpcData.Image_Head; }
	
	FORCEINLINE UUpDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }
	FORCEINLINE UDialogueVoice* GetDialogueVoice() const { return DialogueVoice; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpDialogueComponent> DialogueComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDialogueVoice> DialogueVoice;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float InteractionSphereRadius = 100.f;

	UPROPERTY(VisibleAnywhere, Category="UP Runtime")
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY(VisibleAnywhere, Category="UP Runtime")
	TObjectPtr<UUpVitalAttributeSet> VitalAttributeSet;

	UPROPERTY(Transient)
	TObjectPtr<AUpAiController> AiController;

	FUpNpcData NpcData;
};
