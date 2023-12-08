// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "UpSquadMemberNavButtonWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberNavButtonWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetImage(UTexture2D* InImage) { Image = InImage; }

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
};
