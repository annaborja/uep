// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UpAiController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpAiController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AUpAiController(const FObjectInitializer& ObjectInitializer);
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAISenseConfig_Sight> SenseConfig_Sight;

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
