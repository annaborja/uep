// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpTargetInteractableDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpTargetInteractableDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FText TargetInteractableName;

	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetRootVisibility() const;
	
	virtual void OnCustomHudSet_Implementation() override;

private:
	UFUNCTION()
	void HandleTargetInteractableDelegate(const AActor* TargetInteractable);
};
