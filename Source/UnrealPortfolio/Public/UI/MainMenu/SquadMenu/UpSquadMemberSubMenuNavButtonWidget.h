// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Basic/UpBasicButtonWidget.h"
#include "UpSquadMemberSubMenuNavButtonWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberSubMenuNavButtonWidget : public UUpBasicButtonWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
};
