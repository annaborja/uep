// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Utils/Enums.h"
#include "UpAnimNotify_Landing.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAnimNotify_Landing : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FName BoneName;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	TEnumAsByte<EUpTraceDirection::Type> TraceDirection = EUpTraceDirection::Down;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	float TraceLength = 100.f;
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	float VolumeMultiplier = 1.f;
};
