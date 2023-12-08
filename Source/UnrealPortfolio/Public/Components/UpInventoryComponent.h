// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "UpInventoryComponent.generated.h"

struct FUpTagSpec;

USTRUCT()
struct FUpItemData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemCategory;
	
	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Image;
};

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
