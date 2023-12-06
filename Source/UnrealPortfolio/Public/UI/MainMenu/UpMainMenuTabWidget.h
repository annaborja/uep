// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "UpMainMenuTabWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMainMenuTabWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetLabel(const FText& InLabel) { Label = InLabel; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Label;
};
