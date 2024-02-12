// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Engine/DataTable.h"
#include "UpAnimNotifyState_HitBox.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpAnimNotifyState_HitBox : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, meta=(RowType="UpHitBoxData"))
	FDataTableRowHandle HitBoxDataRowHandle;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
};
