// Copyright AB. All Rights Reserved.

#include "Components/UpInventoryComponent.h"

#include "Tags/ItemTags.h"

bool UUpInventoryComponent::IsItemTag(const FGameplayTag& InTag)
{
	return InTag.MatchesTag(TAG_Item);
}

UUpInventoryComponent::UUpInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
