// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpDialogueOverlayWidget.generated.h"

class AUpNpcCharacter;
struct FUpDialogueOptionData;
struct FUpDialogueStepData;
class UCommonActivatableWidgetSwitcher;
class UUpDialogueStepDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueOverlayWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	UUpDialogueStepDisplayWidget* GetDialogueStepDisplay() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetWidgetSwitcher() const;
	
	void DisplayDialogueStep(AUpNpcCharacter* Npc, const FUpDialogueStepData& DialogueStep) const;
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayDialogueOptions(const AUpNpcCharacter* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions);

protected:
	virtual void NativePreConstruct() override;

private:
	UFUNCTION()
	void ActivateDialogueStepDisplay() const;
};
