// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UpAiController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpAiController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AUpAiController(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
