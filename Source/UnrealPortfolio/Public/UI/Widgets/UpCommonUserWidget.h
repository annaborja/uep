// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Interfaces/UpHudSettable.h"
#include "UpCommonUserWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpCommonUserWidget : public UCommonUserWidget, public IUpHudSettable
{
	GENERATED_BODY()

public:
	virtual void SetCustomHud(AUpHud* InCustomHud) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AUpHud> CustomHud;

	UFUNCTION(BlueprintImplementableEvent)
	TArray<UCommonUserWidget*> GetHudSettableChildren() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnCustomHudSet();
};
