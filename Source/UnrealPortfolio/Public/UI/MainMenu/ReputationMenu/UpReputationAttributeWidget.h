// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpReputationAttributeWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpReputationAttributeWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetLabel(const FText& InLabel) { Label = InLabel;  }
	
	void PopulateValue(const int32 InValue, const int32 MinValue, const int32 MaxValue);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FText Label;
	UPROPERTY(BlueprintReadOnly)
	int32 Value;
	
	UPROPERTY(BlueprintReadOnly)
	float ProgressBarPercentage;
	UPROPERTY(BlueprintReadOnly)
	FLinearColor ProgressBarColor;

	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FLinearColor ProgressBarColorPositive = FLinearColor(FVector(0.008513, 0.328125, 0.069941));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FLinearColor ProgressBarColorNegative = FLinearColor(FVector(0.369792, 0.021746, 0.003346));
};
