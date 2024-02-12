// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotifyState_HitBox.h"

#include "Characters/UpCharacter.h"

void UUpAnimNotifyState_HitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const auto Owner = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		Owner->AddActiveHitBox(HitBoxDataRowHandle);
	}
}

void UUpAnimNotifyState_HitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (const auto Owner = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		Owner->RemoveActiveHitBox(HitBoxDataRowHandle);
	}
}
