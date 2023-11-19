// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "UpMainMenuTabWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpMainMenuTabWidget : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetLabel(const FText& InLabel) { Label = InLabel; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Label;
};
