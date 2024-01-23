// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Utils/Constants.h"
#include "UpAnimNotify_WeaponVfx.generated.h"

class UNiagaraSystem;

UCLASS()
class UNREALPORTFOLIO_API UUpAnimNotify_WeaponVfx : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	FName SocketName = FName(SOCKET_NAME_ATTACK_SOURCE);
	
	UPROPERTY(EditInstanceOnly, Category="UP Params")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
};
