// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpDialogueComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpDialogueStepDisplayWidget.generated.h"

class AUpPlayableNpc;

UCLASS()
class UNREALPORTFOLIO_API UUpDialogueStepDisplayWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void DisplayDialogueStep(AUpPlayableNpc* InNpc, const FUpDialogueStepData& InDialogueStep);

protected:
	UPROPERTY(BlueprintReadOnly)
	FText DialogueLineText;
	UPROPERTY(BlueprintReadOnly)
	FText NpcName;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void HandleContinueAction() const;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> CurrentDialogueLineAudio;
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableNpc> Npc;

	TScriptDelegate<FWeakObjectPtr> OnMontageEndedDelegate;
	
	uint8 CurrentDialogueLineIndex = 0;
	uint8 CurrentMontageIndex = 0;
	FUpDialogueStepData DialogueStep;

	UFUNCTION()
	void AdvanceDialogueLine();
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void DisplayDialogueLine();
	void StopAudio() const;
};
