// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpCharacterSwitcherWidget.generated.h"

class UCommonButtonBase;
class UUpCharacterSwitcherButtonWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterSwitcherWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnActivated() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<UUpCharacterSwitcherButtonWidget*> GetCharacterSwitcherButtons() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonButtonBase* GetDefaultFocusButton() const;

private:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugOverlapSphere = false;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	int32 OverlapSphereRadius = 10000;
	
	void PopulateCharacterSwitcherButtons() const;
};
