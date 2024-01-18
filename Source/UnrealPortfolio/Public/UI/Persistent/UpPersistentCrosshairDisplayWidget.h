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
	float CrosshairSpread_Min = 24.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float CrosshairSpread_Max = CrosshairSpread_Min * 3;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float InterpSpeed_Opacity = 5.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float InterpSpeed_Spread = 300.f;

	float CrosshairOpacity = 1.f;
	float CrosshairSpread = CrosshairSpread_Min;
	
	void UpdateCrosshair() const;
};
