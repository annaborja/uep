// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpPlayerReputationComponent.h"
#include "Components/ActorComponent.h"
#include "UpPlayerPartyComponent.generated.h"

USTRUCT()
struct FUpPartyMembershipSpec
{
	GENERATED_BODY()

	bool IsValid() const { return PlayerReputationData.IsValid(); }

	UPROPERTY(EditAnywhere)
	FUpReputationData PlayerReputationData;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpPlayerPartyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool IsPartyMembershipSpecSatisfied(const FUpPartyMembershipSpec& PartyMembershipSpec, const FUpReputationData& PlayerReputationData);
	
	static void OnPlayerTagSpecGranted(const UObject* WorldContextObject, const FUpTagSpec& GrantedTagSpec);
	
	UUpPlayerPartyComponent();
};
