// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpVitalAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Tags/AttributeTags.h"

UUpVitalAttributeSet::UUpVitalAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Vital_Health, GetHealthAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_MaxHealth, GetMaxHealthAttribute);
}

void UUpVitalAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}
