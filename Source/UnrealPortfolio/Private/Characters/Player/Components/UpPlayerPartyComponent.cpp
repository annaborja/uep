// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerPartyComponent.h"

#include "UpGameInstance.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool UUpPlayerPartyComponent::IsPartyMembershipSpecSatisfied(const FUpPartyMembershipSpec& PartyMembershipSpec, const FUpReputationData& PlayerReputationData)
{
	if (!PartyMembershipSpec.IsValid()) return false;

	UE_LOG(LogTemp, Warning, TEXT("Player affection: %d, spec affection: %d"), PlayerReputationData.Affection, PartyMembershipSpec.PlayerReputationData.Affection)
	UE_LOG(LogTemp, Warning, TEXT("Player esteem: %d, spec esteem: %d"), PlayerReputationData.Esteem, PartyMembershipSpec.PlayerReputationData.Esteem)
	if (PlayerReputationData.Affection < PartyMembershipSpec.PlayerReputationData.Affection) return false;
	if (PlayerReputationData.Esteem < PartyMembershipSpec.PlayerReputationData.Esteem) return false;

	return true;
}

void UUpPlayerPartyComponent::OnPlayerTagSpecGranted(const UObject* WorldContextObject, const FUpTagSpec& GrantedTagSpec)
{
	if (!UUpPlayerReputationComponent::IsReputationTagSpec(GrantedTagSpec)) return;
	
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
