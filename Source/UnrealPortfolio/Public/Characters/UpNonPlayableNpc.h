// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "UpNonPlayableNpc.generated.h"

class UCapsuleComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpNonPlayableNpc : public AUpCharacter
{
	GENERATED_BODY()

public:
	explicit AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> AimAssistCapsule_Body;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> AimAssistCapsule_Head;

	UPROPERTY(EditAnywhere, Category="UP Params")
	float AimAssistHeadHeight= 12.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float AimAssistHeadRadius = 16.f;
};
