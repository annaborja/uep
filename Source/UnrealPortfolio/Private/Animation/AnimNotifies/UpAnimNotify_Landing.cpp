// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotify_Landing.h"

#include "Characters/UpCharacter.h"

void UUpAnimNotify_Landing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (const auto Character = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		Character->HandleLanding(BoneName, TraceDirection, TraceLength, VolumeMultiplier);
	}
}
