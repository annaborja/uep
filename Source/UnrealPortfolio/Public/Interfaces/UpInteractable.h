// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpInteractable.generated.h"

class AUpPlayerController;
struct FUpInteractionData;

UINTERFACE(MinimalAPI)
class UUpInteractable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpInteractable
{
	GENERATED_BODY()

public:
	virtual FUpInteractionData GetInteractionData(const AUpPlayerController* PlayerController) = 0;
	virtual void Interact(AUpPlayerController* PlayerController) = 0;
};
