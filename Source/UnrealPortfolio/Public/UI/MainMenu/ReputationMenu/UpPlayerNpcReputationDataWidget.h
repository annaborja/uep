// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPlayerNpcReputationDataWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpPlayerNpcReputationDataWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetNpcName(const FText& InNpcName) { NpcName = InNpcName;  }
	void SetReputationData(const FUpReputationData& InReputationData) { ReputationData = InReputationData;  }

	FORCEINLINE FText GetNpcName() const { return NpcName; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText NpcName;
	UPROPERTY(BlueprintReadOnly)
	FUpReputationData ReputationData;
};
