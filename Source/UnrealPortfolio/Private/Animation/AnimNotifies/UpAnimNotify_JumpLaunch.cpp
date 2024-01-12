// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotify_JumpLaunch.h"

#include "Characters/UpCharacter.h"

void UUpAnimNotify_JumpLaunch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const auto Character = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		Character->HandleJumpLaunch();
	}
}
