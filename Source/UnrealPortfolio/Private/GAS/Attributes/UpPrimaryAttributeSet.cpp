// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpPrimaryAttributeSet.h"

#include "Tags/AttributeTags.h"

UUpPrimaryAttributeSet::UUpPrimaryAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Primary_Creativity, GetCreativityAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Dexterity, GetDexterityAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Resilience, GetResilienceAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Speed, GetSpeedAttribute);
	TagAttributeMap.Add(TAG_Attribute_Primary_Strength, GetStrengthAttribute);
}
