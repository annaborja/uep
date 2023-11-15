// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Interfaces/UpHudSettable.h"
#include "UpCommonActivatableWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpCommonActivatableWidget : public UCommonActivatableWidget, public IUpHudSettable
{
	GENERATED_BODY()

public:
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	virtual void SetCustomHud(AUpHud* InCustomHud) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AUpHud> CustomHud;
	
	UFUNCTION(BlueprintImplementableEvent)
	TArray<UCommonUserWidget*> GetHudSettableChildren() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnCustomHudSet();

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	ECommonInputMode InputMode = ECommonInputMode::Menu;
};
