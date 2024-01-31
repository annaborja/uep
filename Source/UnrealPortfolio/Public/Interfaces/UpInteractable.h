// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UpInteractable.generated.h"

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
	virtual FUpInteractionData GetInteractionData(const AController* Controller) = 0;
	virtual bool Interact(AController* Controller) = 0;
	virtual void OnInteractionEnd(AController* Controller) {};
};
