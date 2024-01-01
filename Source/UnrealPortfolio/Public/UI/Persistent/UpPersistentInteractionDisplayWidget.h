// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Structs.h"
#include "UpPersistentInteractionDisplayWidget.generated.h"

class URichTextBlock;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentInteractionDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpInteractionData InteractionData;

	bool bCanInteract = false;

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetRootVisibility() const;
	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetImageVisibility() const;

	UFUNCTION(BlueprintImplementableEvent)
	URichTextBlock* GetPromptTextWidget() const;
	
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UFUNCTION()
	void HandleInteractionDataDelegate(const FUpInteractionData InInteractionData);
};
