// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "UpInventoryComponent.generated.h"

USTRUCT()
struct FUpItemData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText Description;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool IsItemTag(const FGameplayTag& InTag);
	
	UUpInventoryComponent();
};
