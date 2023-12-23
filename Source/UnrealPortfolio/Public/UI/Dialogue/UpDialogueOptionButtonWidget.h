// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpDialogueComponent.h"
#include "UI/UpCommonButtonBase.h"
#include "UpDialogueOptionButtonWidget.generated.h"

class AUpPlayableNpc;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueOptionButtonWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetDialogueOption(const FUpDialogueOptionData& InDialogueOption);
	
	void SetNpc(AUpPlayableNpc* InNpc) { Npc = InNpc; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText DialogueOptionText;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableNpc> Npc;
	
	FUpDialogueOptionData DialogueOption;
};
