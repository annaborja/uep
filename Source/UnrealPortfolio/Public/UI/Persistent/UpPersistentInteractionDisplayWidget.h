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

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetRootVisibility() const;

	UFUNCTION(BlueprintImplementableEvent)
	URichTextBlock* GetPromptTextWidget() const;
	UFUNCTION(BlueprintImplementableEvent)
	URichTextBlock* GetPromptSubTextWidget() const;
	
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UFUNCTION()
	void HandleInteractionDataDelegate(const FUpInteractionData InInteractionData);
};
