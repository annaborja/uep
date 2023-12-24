// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "UpNonPlayableNpc.generated.h"

class AUpAiController;
class UBehaviorTree;

UCLASS()
class UNREALPORTFOLIO_API AUpNonPlayableNpc : public AUpCharacter
{
	GENERATED_BODY()

public:
	explicit AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(Transient)
	TObjectPtr<AUpAiController> AiController;
};
