// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpPowerWheelWidget.h"
#include "UI/UpCommonButtonBase.h"
#include "Utils/Structs.h"
#include "UpPowerWheelButtonWidget.generated.h"

UENUM(BlueprintType)
namespace EUpPowerWheelButtonState
{
	enum Type : uint8
	{
		Empty,
		Unavailable,
		Available,
		Active
	};
}

UCLASS()
class UNREALPORTFOLIO_API UUpPowerWheelButtonWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetSpecialMoveData(const FUpSpecialMoveData InSpecialMoveData);

	FUpSpecialMoveData GetSpecialMoveData() const { return SpecialMoveData; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpSpecialMoveData SpecialMoveData;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EUpPowerWheelButtonState::Type> PowerWheelButtonState = EUpPowerWheelButtonState::Empty;
	
	virtual void NativePreConstruct() override;
	virtual void NativeOnHovered() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USoundBase> Sfx_Focus;
};
