// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Utils/Structs.h"
#include "UpInventoryComponent.generated.h"

struct FUpTagSpec;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool IsInventoryTag(const FGameplayTag& Tag);
	static bool ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec);
	static bool HandleTagSpecGrant(const UObject* WorldContextObject, const FUpTagSpec& TagSpec);
	
	UUpInventoryComponent();
};
