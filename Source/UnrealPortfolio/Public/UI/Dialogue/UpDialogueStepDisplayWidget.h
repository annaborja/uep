// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpNpcCharacter.h"
#include "Components/UpDialogueComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpDialogueStepDisplayWidget.generated.h"

class AUpNpcCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueStepDisplayWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void DisplayDialogueStep(AUpNpcCharacter* InNpc, const FUpDialogueStepData& InDialogueStep);

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	FText DialogueLineText;
	UPROPERTY(Transient, BlueprintReadOnly)
	FText NpcName;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void HandleContinueAction() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> CurrentDialogueLineAudio;
	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> Npc;
	
	uint8 CurrentDialogueLineIndex = 0;
	FUpDialogueStepData DialogueStep;

	UFUNCTION()
	void AdvanceDialogueLine();

	void DisplayDialogueLine();
	void StopAudio() const;
};
