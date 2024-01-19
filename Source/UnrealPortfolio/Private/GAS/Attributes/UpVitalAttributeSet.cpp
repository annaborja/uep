// Copyright AB. All Rights Reserved.

#include "GAS/Attributes/UpVitalAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "UpGameInstance.h"
#include "GAS/UpGasDataAsset.h"
#include "Interfaces/UpCombatable.h"
#include "Tags/AttributeTags.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpVitalAttributeSet::UUpVitalAttributeSet()
{
	TagAttributeMap.Add(TAG_Attribute_Vital_Health, GetHealthAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_MaxHealth, GetMaxHealthAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_Shield, GetShieldAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_MaxShield, GetMaxShieldAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_Stamina, GetStaminaAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_MaxStamina, GetMaxStaminaAttribute);
	TagAttributeMap.Add(TAG_Attribute_Vital_IncomingDamage, GetIncomingDamageAttribute);
}

void UUpVitalAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	} else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.f, GetMaxShield()));
	} else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));

		if (Data.EvaluatedData.Magnitude < 0.f)
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
			{
				if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
				{
					if (const auto EffectClass = GasDataAsset->GetEffectClass_StaminaRegenCooldown())
					{
						Data.Target.ApplyGameplayEffectSpecToSelf(
							*Data.Target.MakeOutgoingSpec(EffectClass, 1.f, Data.Target.MakeEffectContext()).Data.Get());
					}
				}
			}
		}
	} else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		if (GetIncomingDamage() > 0.f)
		{
			SetHealth(FMath::Clamp(GetHealth() - GetIncomingDamage(), 0.f, GetMaxHealth()));

			if (GetHealth() <= 0.f)
			{
				if (const auto TargetAbilitySystemComponent = &Data.Target; IsValid(TargetAbilitySystemComponent))
				{
					if (const auto Combatable = Cast<IUpCombatable>(TargetAbilitySystemComponent->GetAvatarActor()))
					{
						Combatable->Die();
					}
				}
			}
		}
		
		SetIncomingDamage(0.f);
	}
}
