// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpPlayerInteractionComponent.generated.h"

class AUpPlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUpPlayerInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE AActor* GetTargetInteractable() const { return TargetInteractable; }
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugInteractionTrace = false;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float TraceLength = 150.f;
	
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	TObjectPtr<AActor> TargetInteractable;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerCharacter> CustomOwner;
};
