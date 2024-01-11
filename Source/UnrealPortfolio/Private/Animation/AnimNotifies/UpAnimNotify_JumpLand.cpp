// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotify_JumpLand.h"

#include "Characters/UpCharacter.h"

void UUpAnimNotify_JumpLand::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const auto Character = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		Character->HandleJumpLand();
	}
}
