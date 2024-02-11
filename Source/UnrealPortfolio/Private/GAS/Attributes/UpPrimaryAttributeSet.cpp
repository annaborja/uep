// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpPrimaryAttributeSet.h"

#include "Tags/GasTags.h"

UUpPrimaryAttributeSet::UUpPrimaryAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Primary_Dexterity, GetDexterityAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Instinct, GetInstinctAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Resilience, GetResilienceAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Speed, GetSpeedAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Strength, GetStrengthAttribute);
}
