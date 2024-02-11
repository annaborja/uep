// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Structs.h"
#include "UpPersistentNotificationDisplayWidget.generated.h"

class UCommonTextStyle;
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
	UPanelWidget* GetTextContainer() const;

	virtual void NativePreConstruct() override;
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UCommonTextStyle> NotificationTextStyle;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDataTable> NotificationTextStyleSet;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	uint8 MaxNotificationsDisplayed = 3;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float NotificationGap = 20.f;
	
	UPROPERTY(Transient)
	TArray<FUpNotificationData> NotificationQueue;
	UPROPERTY(Transient)
	TArray<FUpNotificationData> DisplayedNotifications;
	
	UFUNCTION()
	void HandleNotification(const FUpNotificationData NotificationData);

	void ProcessNotificationQueue();
	void UpdateDisplayedText() const;
};
