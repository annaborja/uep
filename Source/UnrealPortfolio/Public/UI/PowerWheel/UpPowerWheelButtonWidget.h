// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "UpPowerWheelButtonWidget.generated.h"

class AUpPlayableNpc;
struct FGameplayTag;

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
	void SetNpc(AUpPlayableNpc* InNpc);
	void SetNpcTag(const FGameplayTag& NpcTag);

protected:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EUpPowerWheelButtonState::Type> PowerWheelButtonState = EUpPowerWheelButtonState::Empty;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
	UPROPERTY(BlueprintReadOnly)
	FText Label;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableNpc> Npc;
};
