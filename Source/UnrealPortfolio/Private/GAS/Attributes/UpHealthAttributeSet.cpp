// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpHealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Tags/AttributeTags.h"

UUpHealthAttributeSet::UUpHealthAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Health_Health, GetHealthAttribute);
	TagAttributeMap.Add(TAG_Attribute_Health_MaxHealth, GetMaxHealthAttribute);
}

void UUpHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}
