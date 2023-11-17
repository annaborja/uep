// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpDialogueOptionsDisplayWidget.generated.h"

class AUpNpcCharacter;
struct FUpDialogueOptionData;
class UUpDialogueOptionButtonWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueOptionsDisplayWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void PopulateDialogueOptions(AUpNpcCharacter* Npc, TArray<FUpDialogueOptionData> DialogueOptions);

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpDialogueOptionButtonWidget> DialogueOptionButtonClass;
	
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetDialogueOptionsContainer() const;
};
