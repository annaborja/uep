// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpAttributeSet.h"

FGameplayAttribute UUpAttributeSet::GetAttribute(const FGameplayTag& Tag) const
{
	const auto AttributeFuncPtr = TagAttributeMap.Find(Tag);
	
	if (AttributeFuncPtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid attribute tag %s"), *Tag.ToString())
		return FGameplayAttribute();
	}

	return (*AttributeFuncPtr)();
}
