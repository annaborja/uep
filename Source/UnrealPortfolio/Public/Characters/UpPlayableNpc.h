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
class UNREALPORTFOLIO_API AUpPlayableNpc : public AUpPlayableCharacter, public IUpInteractable, public IUpNameable
{
	GENERATED_BODY()

public:
	explicit AUpPlayableNpc(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Die() override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual FUpInteractionData GetInteractionData(const AController* Controller) override;
	virtual bool Interact(AController* Controller) override;

	virtual FText GetInGameName() const override { return CharacterData.Name; }

	void JumpToLocation(const FVector& TargetLocation, const float Duration);
	bool Mantle() const;

	FORCEINLINE UUpDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpDialogueComponent> DialogueComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
};
