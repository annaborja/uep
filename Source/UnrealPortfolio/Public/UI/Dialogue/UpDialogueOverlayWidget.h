// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpDialogueOverlayWidget.generated.h"

class AUpPlayableNpc;
struct FUpDialogueOptionData;
struct FUpDialogueStepData;
class UCommonActivatableWidgetSwitcher;
class UUpDialogueOptionsDisplayWidget;
class UUpDialogueStepDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueOverlayWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	UUpDialogueOptionsDisplayWidget* GetDialogueOptionsDisplay() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpDialogueStepDisplayWidget* GetDialogueStepDisplay() const;
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActivatableWidgetSwitcher* GetWidgetSwitcher() const;
	
	void DisplayDialogueStep(AUpPlayableNpc* Npc, const FUpDialogueStepData& DialogueStep) const;
	void DisplayDialogueOptions(AUpPlayableNpc* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions) const;

protected:
	virtual void NativeOnDeactivated() override;

private:
	UFUNCTION()
	void ActivateDialogueStepDisplay() const;
};
