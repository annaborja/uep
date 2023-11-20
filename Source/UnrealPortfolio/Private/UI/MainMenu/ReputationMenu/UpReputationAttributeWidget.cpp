// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/ReputationMenu/UpReputationAttributeWidget.h"

void UUpReputationAttributeWidget::PopulateValue(const int32 InValue, const int32 MinValue, const int32 MaxValue)
{
	Value = InValue;
	ProgressBarPercentage = static_cast<float>(Value - MinValue) / static_cast<float>(MaxValue - MinValue);
	ProgressBarColor = Value > (MaxValue - MinValue) / 2 + MinValue ? ProgressBarColorPositive : ProgressBarColorNegative;
}
