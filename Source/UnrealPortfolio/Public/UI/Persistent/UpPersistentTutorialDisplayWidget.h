// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentTutorialDisplayWidget.generated.h"

struct FUpTutorialData;
class UCommonActionWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentTutorialDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Text;

	UFUNCTION(BlueprintImplementableEvent)
	UCommonActionWidget* GetCommonActionWidget() const;
	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* GetFadeAnimation() const;

	virtual void NativePreConstruct() override;
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	FTimerHandle HideTimerHandle;
	
	UFUNCTION()
	void HandleTutorial(const FUpTutorialData& TutorialData);
	UFUNCTION()
	void ClearData();
};
