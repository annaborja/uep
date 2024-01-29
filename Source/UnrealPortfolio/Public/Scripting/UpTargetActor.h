// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UpTagIdable.h"
#include "UpTargetActor.generated.h"

class USphereComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpTargetActor : public AActor, public IUpTagIdable
{
	GENERATED_BODY()
	
public:	
	AUpTargetActor();

	virtual FGameplayTag GetTagId() const override { return TagId; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag TagId;
};
