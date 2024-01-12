// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UpAnimNotify_JumpLaunch.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAnimNotify_JumpLaunch : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
