// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "UpInvisibleButtonWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpInvisibleButtonWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
};
