// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentCrosshairDisplayWidget.generated.h"

class UBorder;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentCrosshairDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetTargetCrosshairSpread(const float InCrosshairSpread) { CrosshairSpread = InCrosshairSpread; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UBorder* GetTopCrosshair() const;
	UFUNCTION(BlueprintImplementableEvent)
	UBorder* GetBottomCrosshair() const;
	UFUNCTION(BlueprintImplementableEvent)
	UBorder* GetLeftCrosshair() const;
	UFUNCTION(BlueprintImplementableEvent)
	UBorder* GetRightCrosshair() const;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float CrosshairLength = 4.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float CrosshairThickness = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float CrosshairSpread = 24.f;

	float TargetCrosshairSpread = CrosshairSpread;
	
	void UpdateCrosshairSpread() const;
};
