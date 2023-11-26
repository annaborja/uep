// Copyright AB. All Rights Reserved.

#include "Components/UpInventoryComponent.h"

#include "Tags/ItemTags.h"
#include "Utils/Structs.h"

bool UUpInventoryComponent::ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec)
{
	return TagSpec.Tag.MatchesTag(TAG_Item);
}

UUpInventoryComponent::UUpInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
