// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpNonPlayableNpc.h"
#include "UpEnemyCharacter.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpEnemyCharacter : public AUpNonPlayableNpc
{
	GENERATED_BODY()

public:
	explicit AUpEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Die() override;

	FORCEINLINE FGameplayTag GetPatrolPathTag() const { return PatrolPathTag; }

	UFUNCTION(BlueprintCallable)
	void SetPatrolPathTag(const FGameplayTag& InTag);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> AimAssistCapsule_Body;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> AimAssistCapsule_Head;

	UPROPERTY(EditAnywhere, Category="UP Params")
	float AimAssistHeadHeight= 12.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float AimAssistHeadRadius = 16.f;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag PatrolPathTag;

	virtual void BeginPlay() override;

	void SetPatrolPath();
};
