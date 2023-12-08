// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpSquadMemberDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetNpcName(const FText& InNpcName) { NpcName = InNpcName;  }

	FORCEINLINE FText GetNpcName() const { return NpcName; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText NpcName;
};
