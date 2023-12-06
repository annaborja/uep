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
	virtual void NativeOnDeactivated() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	TArray<UUpCharacterSwitcherButtonWidget*> GetCharacterSwitcherButtons() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonButtonBase* GetDefaultFocusButton() const;
	UFUNCTION(BlueprintCallable)
	void HandleAnyKeyReleased(const FKey& Key);

private:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugOverlapSphere = false;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	int32 OverlapSphereRadius = 10000;
	
	bool bCanClose = false;

	void PopulateCharacterSwitcherButtons() const;
};
