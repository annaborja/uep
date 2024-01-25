// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UpAnimNotify_GameplayEvent.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAnimNotify_GameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FGameplayTag EventTag;
};
