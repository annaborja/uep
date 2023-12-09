// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpPlayerReputationComponent.generated.h"

struct FGameplayTag;
struct FUpTagSpec;

USTRUCT(BlueprintType)
struct FUpReputationData
{
	GENERATED_BODY()

	bool IsValid() const { return Affection >= 0 && Esteem >= 0; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Affection = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Esteem = -1;
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
	
	static bool IsReputationTag(const FGameplayTag& Tag);
	static bool ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec);
	static bool HandleTagSpecGrant(const UObject* WorldContextObject, const FUpTagSpec& TagSpec);
	
	UUpPlayerReputationComponent();
};
