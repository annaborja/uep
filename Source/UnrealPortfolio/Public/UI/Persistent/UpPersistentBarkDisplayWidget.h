// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpDialogueComponent.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentBarkDisplayWidget.generated.h"

class AUpCharacter;
class UDialogueVoice;
class URichTextBlock;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentBarkDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetRootVisibility() const;
	UFUNCTION(BlueprintImplementableEvent)
	URichTextBlock* GetTextWidget() const;
	
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> CurrentDialogueLineAudio;
	UPROPERTY(Transient)
	TObjectPtr<UDialogueVoice> DialogueVoice;
	UPROPERTY(Transient)
	TObjectPtr<AUpCharacter> SpeakerActor;

	FUpBarkData BarkData;
	FText SpeakerName;
	
	TScriptDelegate<FWeakObjectPtr> OnMontageEndedDelegate;

	uint8 CurrentDialogueLineIdx = 0;
	uint8 CurrentMontageIdx = 0;
	bool bMontagePlayed = false;
	bool bShow = false;
	
	UFUNCTION()
	void AdvanceDialogueLine();
	UFUNCTION()
	void HandleBarkBroadcast(const FUpBarkData& InBarkData);
	UFUNCTION()
	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void DisplayDialogueLine();
	void StopAudio() const;
};
