// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "Utils/Structs.h"
#include "UpObjectiveDisplayWidget.generated.h"

class UCommonTextBlock;
class UCommonTextStyle;

DECLARE_MULTICAST_DELEGATE(FUpObjectiveDisplayWidgetFocusDelegate);

UCLASS()
class UNREALPORTFOLIO_API UUpObjectiveDisplayWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	FUpObjectiveDisplayWidgetFocusDelegate FocusDelegate;
	
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	
	void SetMissionData(const FUpMissionData InMissionData) { MissionData = InMissionData; }
	void SetObjectiveData(const FUpObjectiveData InObjectiveData) { ObjectiveData = InObjectiveData; }
	void SetCompleted(const bool bInCompleted);

protected:
	UPROPERTY(BlueprintReadOnly)
	FUpMissionData MissionData;
	UPROPERTY(BlueprintReadOnly)
	FUpObjectiveData ObjectiveData;
	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;

	UFUNCTION(BlueprintImplementableEvent)
	UCommonTextBlock* GetSummaryTextBlock() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UCommonButtonStyle> ButtonStyle;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UCommonButtonStyle> FocusedButtonStyle;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UCommonTextStyle> ObjectiveSummaryTextStyle;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UCommonTextStyle> ObjectiveSummaryCompletedTextStyle;
};
