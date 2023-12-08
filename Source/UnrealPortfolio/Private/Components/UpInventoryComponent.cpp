// Copyright AB. All Rights Reserved.

#include "Components/UpInventoryComponent.h"

#include "Tags/ItemTags.h"
#include "Utils/Structs.h"

bool UUpInventoryComponent::IsInventoryTag(const FGameplayTag& Tag)
{
	return Tag.MatchesTag(TAG_Item);
}

bool UUpInventoryComponent::ShouldHandleTagSpecGrant(const FUpTagSpec& TagSpec)
{
	return IsInventoryTag(TagSpec.Tag);
}

bool UUpInventoryComponent::HandleTagSpecGrant(const UObject* WorldContextObject, const FUpTagSpec& TagSpec)
{
	return true;
}

UUpInventoryComponent::UUpInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
