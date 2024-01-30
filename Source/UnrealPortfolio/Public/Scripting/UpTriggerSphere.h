// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Scripting/UpTriggerVolume.h"
#include "UpTriggerSphere.generated.h"

class USphereComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpTriggerSphere : public AUpTriggerVolume
{
	GENERATED_BODY()

public:
	AUpTriggerSphere();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
};
