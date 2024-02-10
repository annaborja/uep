// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Structs.h"
#include "UpPersistentNotificationDisplayWidget.generated.h"

class URichTextBlock;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentNotificationDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Text;

	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* GetFadeAnimation() const;
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetTabsContainer() const;

	virtual void NativePreConstruct() override;
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	uint8 MaxNumNotificationsDisplayed = 3;
	
	UPROPERTY(Transient)
	TArray<FUpNotificationData> NotificationQueue;
	UPROPERTY(Transient)
	TArray<FUpNotificationData> DisplayedNotifications;
	
	UFUNCTION()
	void HandleNotification(const FUpNotificationData NotificationData);

	void ProcessNotificationQueue();
	void UpdateTextWidget() const;
};
