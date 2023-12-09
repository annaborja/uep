// Copyright AB. All Rights Reserved.

#include "UI/Basic/UpAttributeBarWidget.h"

#include "GAS/Attributes/UpAttributeSet.h"

void UUpAttributeBarWidget::PopulateAttribute(const FGameplayTag& Tag, const UUpAttributeSet* AttributeSet)
{
	const auto Attribute = AttributeSet->GetAttribute(Tag);

	if (!Attribute.IsValid()) return;

	Name = FText::FromString(Attribute.AttributeName);
	Value = Attribute.GetNumericValue(AttributeSet);
}
