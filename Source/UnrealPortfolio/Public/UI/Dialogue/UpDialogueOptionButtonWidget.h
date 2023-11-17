// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Components/UpDialogueComponent.h"
#include "UpDialogueOptionButtonWidget.generated.h"

class AUpHud;
class AUpNpcCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueOptionButtonWidget : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetDialogueOption(const FUpDialogueOptionData& InDialogueOption);
	
	void SetCustomHud(AUpHud* InCustomHud) { CustomHud = InCustomHud; }
	void SetNpc(AUpNpcCharacter* InNpc) { Npc = InNpc; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText DialogueOptionText;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AUpHud> CustomHud;
	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> Npc;
	
	FUpDialogueOptionData DialogueOption;
};
