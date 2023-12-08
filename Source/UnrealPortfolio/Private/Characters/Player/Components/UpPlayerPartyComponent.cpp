// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerPartyComponent.h"

#include "UpGameInstance.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool UUpPlayerPartyComponent::IsPartyMembershipSpecSatisfied(const FUpPartyMembershipSpec& PartyMembershipSpec, const FUpReputationData& PlayerReputationData)
{
	if (!PartyMembershipSpec.IsValid()) return false;

	if (PlayerReputationData.Affection < PartyMembershipSpec.PlayerReputationData.Affection) return false;
	if (PlayerReputationData.Esteem < PartyMembershipSpec.PlayerReputationData.Esteem) return false;

	return true;
}

void UUpPlayerPartyComponent::OnPlayerTagSpecGranted(const UObject* WorldContextObject, const FUpTagSpec& GrantedTagSpec)
{
	if (!UUpPlayerReputationComponent::IsReputationTag(GrantedTagSpec.Tag)) return;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(WorldContextObject))
	{
		for (const auto PartyMembershipSpecMapping : GameInstance->GetPartyMembershipSpecMap())
		{
			if (IsPartyMembershipSpecSatisfied(PartyMembershipSpecMapping.Value, GameInstance->GetPlayerReputationData(PartyMembershipSpecMapping.Key)))
			{
				GameInstance->AddPartyMember(PartyMembershipSpecMapping.Key);
			} else
			{
				GameInstance->RemovePartyMember(PartyMembershipSpecMapping.Key);
			}
		}
	}
}

UUpPlayerPartyComponent::UUpPlayerPartyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
