// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/StatsMenu/UpPrimaryAttributeDisplayWidget.h"

#include "GAS/Attributes/UpPrimaryAttributeSet.h"

void UUpPrimaryAttributeDisplayWidget::PopulateAttribute(const FGameplayTag& Tag, const UUpPrimaryAttributeSet* AttributeSet)
{
	const auto Attribute = AttributeSet->GetAttribute(Tag);

	if (!Attribute.IsValid()) return;

	Name = FText::FromString(Attribute.AttributeName);
	Value = Attribute.GetNumericValue(AttributeSet);
}
