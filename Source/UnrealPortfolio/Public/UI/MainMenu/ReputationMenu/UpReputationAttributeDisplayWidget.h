// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpReputationAttributeDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpReputationAttributeDisplayWidget : public UUpCommonUserWidget
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
	FLinearColor ProgressBarColorPositive = FLinearColor(FVector(0.008513f, 0.328125f, 0.069941f));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FLinearColor ProgressBarColorNegative = FLinearColor(FVector(0.369792f, 0.021746f, 0.003346f));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FLinearColor ProgressBarColorNeutral = FLinearColor(FVector(0.260417f, 0.260417f, 0.260417f));
};
