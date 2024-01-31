// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/UpItem.h"
#include "UpLadder.generated.h"

class AUpCharacter;
class UBoxComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpLadder : public AUpItem
{
	GENERATED_BODY()

public:
	AUpLadder();
	
	virtual bool Interact(AController* Controller) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> InteractionBox;

	virtual bool CanInteract(const AController* Controller) const override;
	virtual FText GetInteractionPromptText(const AController* Controller) const override;

	bool IsCharacterClimbing(const AUpCharacter* Character) const;
};
