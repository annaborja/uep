// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpPowerWheelWidget.generated.h"

class UCommonButtonBase;
class UUpPowerWheelButtonWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPowerWheelWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<UUpPowerWheelButtonWidget*> GetPowerWheelButtons() const;

private:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugOverlapSphere = false;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	int32 OverlapSphereRadius = 10000;
	
	void PopulatePowerWheelButtons() const;
};
