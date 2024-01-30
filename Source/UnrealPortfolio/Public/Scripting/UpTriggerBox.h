// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpTriggerVolume.h"
#include "UpTriggerBox.generated.h"

class UBoxComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpTriggerBox : public AUpTriggerVolume
{
	GENERATED_BODY()

public:
	AUpTriggerBox();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
};
