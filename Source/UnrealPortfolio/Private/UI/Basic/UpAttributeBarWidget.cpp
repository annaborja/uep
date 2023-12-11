// Copyright AB. All Rights Reserved.

#include "UI/Basic/UpAttributeBarWidget.h"

#include "GAS/Attributes/UpAttributeSet.h"

void UUpAttributeBarWidget::PopulateAttribute(const FGameplayTag& Tag, const UUpAttributeSet* AttributeSet)
{
	const auto Attribute = AttributeSet->GetAttribute(Tag);

	if (!Attribute.IsValid()) return;

	constexpr int32 MinValue = 0;
	constexpr int32 MaxValue = 100;

	Name = FText::FromString(Attribute.AttributeName);
	Value = Attribute.GetNumericValue(AttributeSet);
	Percentage = (Value - MinValue) / static_cast<float>(MaxValue - MinValue);
}
