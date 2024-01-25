// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotify_GameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"

void UUpAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const auto Actor = MeshComp->GetOwner();

	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor); AbilitySystemInterface && AbilitySystemInterface->GetAbilitySystemComponent())
	{
		const FGameplayEventData EventPayload;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, EventPayload);
	}
}
