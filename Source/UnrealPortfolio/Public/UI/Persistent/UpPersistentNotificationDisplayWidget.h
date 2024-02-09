// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentNotificationDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentNotificationDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Text;

	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* GetFadeAnimation() const;

	virtual void NativePreConstruct() override;
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	FTimerHandle HideTimerHandle;
	
	UFUNCTION()
	void HandleNotification(const FUpNotificationData& NotificationData);
	UFUNCTION()
	void ClearData();
};
