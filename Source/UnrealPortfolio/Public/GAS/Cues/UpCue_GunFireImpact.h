// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UpCue_GunFireImpact.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpCue_GunFireImpact : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UUpCue_GunFireImpact();
	
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
