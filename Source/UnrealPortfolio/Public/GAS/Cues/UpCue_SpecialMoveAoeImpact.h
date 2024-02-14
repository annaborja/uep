// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "UpCue_SpecialMoveAoeImpact.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpCue_SpecialMoveAoeImpact : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UUpCue_SpecialMoveAoeImpact();
	
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
