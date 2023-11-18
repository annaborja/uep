// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils/GeneralStructs.h"
#include "UpReputationComponent.generated.h"

struct FUpTagSpec;

USTRUCT(BlueprintType)
struct FUpRelationshipData
{
	GENERATED_BODY()
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpReputationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec);
	
	UUpReputationComponent();

	bool HandleTagSpecGrant(const FUpTagSpec& TagSpec) const;
};
