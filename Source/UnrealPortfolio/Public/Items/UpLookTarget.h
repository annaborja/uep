// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UpTagIdable.h"
#include "UpLookTarget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API AUpLookTarget : public AActor, public IUpTagIdable
{
	GENERATED_BODY()
	
public:	
	AUpLookTarget();

	virtual FGameplayTag GetTagId() const override { return TagId; }

protected:
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag TagId;
};
