// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Structs.h"
#include "UpPersistentSpecialMoveDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentSpecialMoveDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FUpSpecialMoveData SpecialMoveData;

private:
	void UpdateValues();
};
