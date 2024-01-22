// Copyright AB. All Rights Reserved.

#include "Animation/AnimNotifies/UpAnimNotify_WeaponVfx.h"

#include "NiagaraFunctionLibrary.h"
#include "Characters/UpCharacter.h"
#include "Items/UpWeapon.h"

void UUpAnimNotify_WeaponVfx::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const auto Character = Cast<AUpCharacter>(MeshComp->GetOwner()))
	{
		if (const auto Weapon = Character->GetActiveWeapon())
		{
			if (const auto Mesh = Weapon->GetStaticMeshComponent())
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, Mesh, SocketName,
					FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
			}
		}
	}
}
