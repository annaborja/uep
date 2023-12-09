// Copyright AB. All Rights Reserved.

#include "UI/Basic/UpProgressBarWidget.h"

void UUpProgressBarWidget::PopulateValue(const int32 InValue, const int32 MinValue, const int32 MaxValue)
{
	Value = InValue;
	ProgressBarPercentage = static_cast<float>(Value - MinValue) / static_cast<float>(MaxValue - MinValue);
	ProgressBarColor = ProgressBarColorNeutral;

	if (const auto ProgressBarHalfwayPoint = (MaxValue - MinValue) / 2 + MinValue; Value > ProgressBarHalfwayPoint)
	{
		ProgressBarColor = ProgressBarColorPositive;
	} else if (Value < ProgressBarHalfwayPoint)
	{
		ProgressBarColor = ProgressBarColorNegative;
	}
}
