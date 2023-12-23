// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpDialogueOptionsDisplayWidget.generated.h"

class AUpPlayableNpc;
struct FUpDialogueOptionData;
class UUpDialogueOptionButtonWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueOptionsDisplayWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void PopulateDialogueOptions(AUpPlayableNpc* Npc, TArray<FUpDialogueOptionData> DialogueOptions);

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpDialogueOptionButtonWidget> DialogueOptionButtonClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float DialogueOptionGap = 4.f;
	
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetDialogueOptionsContainer() const;
	
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
};
