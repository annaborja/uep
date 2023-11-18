// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpReputationComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpReputationMenuWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpReputationMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerKarma = -1;
	UPROPERTY(BlueprintReadOnly)
	FUpRelationshipData PlayerRelationshipData;
};
