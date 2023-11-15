// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpInteractable.generated.h"

class AUpPlayerController;

UINTERFACE(MinimalAPI)
class UUpInteractable : public UInterface
{
	GENERATED_BODY()
};

class UNREALPORTFOLIO_API IUpInteractable
{
	GENERATED_BODY()

public:
	virtual bool CanInteract() const = 0;
	virtual void Interact(AUpPlayerController* PlayerController) = 0;
};
