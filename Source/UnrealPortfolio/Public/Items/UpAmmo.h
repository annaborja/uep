// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/UpItem.h"
#include "UpAmmo.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpAmmo : public AUpItem
{
	GENERATED_BODY()

protected:
	virtual FText GetInteractionPrompt() const override;
	virtual int32 GetInteractionQuantity(const AUpPlayerController* PlayerController, const FGameplayTag& DynamicRelatedTag) const override;
	virtual FGameplayTag GetInteractionRelatedTag(const AUpPlayerController* PlayerController) const override;
};
