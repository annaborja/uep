// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "UpPlayableCharacter.h"
#include "Engine/DataTable.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpPlayableNpc.generated.h"

class AUpAiController;
class UBehaviorTree;
class UDialogueVoice;
class USphereComponent;
class UUpDialogueComponent;

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
class UNREALPORTFOLIO_API AUpPlayableNpc : public AUpPlayableCharacter, public IGameplayTagAssetInterface,
	public IUpInteractable, public IUpNameable, public IUpTagIdable, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayableNpc(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnEquipmentActivation(const FUpItemData& ItemData) override;
	virtual void OnEquipmentDeactivation(const FUpItemData& ItemData) override;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual bool CanInteract() const override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return NpcData.Name; }

	virtual FGameplayTag GetTagId() const override { return TagId; }

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	void JumpToLocation(const FVector& TargetLocation, const float Duration);
	bool Mantle() const;
	void ToggleSprint(const bool bSprint) const;

	FORCEINLINE FUpNpcData GetNpcData() const { return NpcData; }
	
	FORCEINLINE UTexture2D* GetImage_FullBody() const { return NpcData.Image_FullBody; }
	FORCEINLINE UTexture2D* GetImage_Head() const { return NpcData.Image_Head; }
	
	FORCEINLINE UUpDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }
	FORCEINLINE UDialogueVoice* GetDialogueVoice() const { return DialogueVoice; }

private:
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

	UPROPERTY(Transient)
	TObjectPtr<AUpAiController> AiController;

	FUpNpcData NpcData;
};
