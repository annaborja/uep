// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpCharacterSwitcherWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterSwitcherWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnDeactivated() override;
	
	UFUNCTION(BlueprintCallable)
	void HandleAnyKeyReleased(const FKey& Key);

private:
	bool bCanClose = false;
};
