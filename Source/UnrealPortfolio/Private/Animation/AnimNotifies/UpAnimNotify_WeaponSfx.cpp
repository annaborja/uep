// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotify_WeaponSfx.h"

#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"

void UUpAnimNotify_WeaponSfx::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const auto Character = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		if (const auto Weapon = Character->GetActiveWeapon())
		{
			if (const auto Sound = Weapon->GetSfx_Fire())
			{
				UGameplayStatics::PlaySoundAtLocation(Weapon, Sound, Weapon->GetActorLocation(), Weapon->GetActorRotation());
			}
		}
	}
}
