// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpPlayableCharacter.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "UpPlayableNpc.generated.h"

class USphereComponent;
class UUpDialogueComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayableNpc : public AUpPlayableCharacter, public IGameplayTagAssetInterface, public IUpInteractable, public IUpNameable
{
	GENERATED_BODY()

public:
	explicit AUpPlayableNpc(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual FUpInteractionData GetInteractionData(const AUpPlayerController* PlayerController) override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return CharacterData.Name; }

	void JumpToLocation(const FVector& TargetLocation, const float Duration);
	bool Mantle() const;
	void ToggleSprint(const bool bSprint) const;

	FORCEINLINE UUpDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpDialogueComponent> DialogueComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float InteractionSphereRadius = 100.f;
};
