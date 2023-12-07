// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpPlayerReputationComponent.generated.h"

struct FUpTagSpec;

USTRUCT(BlueprintType)
struct FUpReputationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Affection = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Esteem = 50;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpPlayerReputationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static constexpr uint8 AffectionMinValue = 0;
	static constexpr uint8 EsteemMinValue = 0;
	
	static constexpr uint8 AffectionMaxValue = 100;
	static constexpr uint8 EsteemMaxValue = 100;
	
	static bool IsReputationTagSpec(const FUpTagSpec& TagSpec);
	static bool ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec);
	static bool HandleTagSpecGrant(const UObject* WorldContextObject, const FUpTagSpec& TagSpec);
	
	UUpPlayerReputationComponent();
};
