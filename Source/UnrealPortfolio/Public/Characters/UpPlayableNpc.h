// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "UpPlayableCharacter.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "UpPlayableNpc.generated.h"

class AUpAiController;
class UBehaviorTree;
class UDialogueVoice;
class USphereComponent;
class UUpDialogueComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayableNpc : public AUpPlayableCharacter, public IGameplayTagAssetInterface,
	public IUpInteractable, public IUpNameable, public IUpTagIdable
{
	GENERATED_BODY()

public:
	explicit AUpPlayableNpc(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual FUpInteractionData GetInteractionData() override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return CharacterData.Name; }

	virtual FGameplayTag GetTagId() const override { return TagId; }

	void JumpToLocation(const FVector& TargetLocation, const float Duration);
	bool Mantle() const;
	void ToggleSprint(const bool bSprint) const;

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
};
