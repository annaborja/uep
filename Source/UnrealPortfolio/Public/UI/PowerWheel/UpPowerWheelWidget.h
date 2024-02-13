// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpPowerWheelWidget.generated.h"

class UUpPowerWheelButtonWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPowerWheelWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<UUpPowerWheelButtonWidget*> GetPowerWheelButtons() const;

protected:
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	void PopulatePowerWheelButtons() const;
};
