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
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float RenderOpacitySelected = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float RenderOpacityDeselected = 0.5f;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;

	virtual void NativePreConstruct() override;
	virtual void NativeOnSelected(bool bBroadcast) override;
	virtual void NativeOnDeselected(bool bBroadcast) override;
};
