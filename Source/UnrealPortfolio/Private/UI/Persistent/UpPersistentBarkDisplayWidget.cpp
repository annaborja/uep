// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentBarkDisplayWidget.h"

#include "UpGameInstance.h"
#include "Components/AudioComponent.h"
#include "Components/RichTextBlock.h"
#include "Components/UpDialogueComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Levels/UpLevelScriptActor.h"
#include "Sound/DialogueWave.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

ESlateVisibility UUpPersistentBarkDisplayWidget::GetRootVisibility() const
{
	if (!bShow) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentBarkDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->BarkDelegate.AddUObject(this, &ThisClass::HandleBarkBroadcast);
}

void UUpPersistentBarkDisplayWidget::AdvanceDialogueLine()
{
	bShow = false;
	
	if (CurrentDialogueLineIdx >= BarkData.DialogueLines.Num() - 1)
	{
		if (SpeakerActor)
		{
			if (const auto Mesh = SpeakerActor->GetMesh())
			{
				if (const auto AnimInstance = Mesh->GetAnimInstance();
					AnimInstance && AnimInstance->OnMontageEnded.Contains(OnMontageEndedDelegate))
				{
					AnimInstance->OnMontageEnded.Remove(OnMontageEndedDelegate);
				}
			}
			
			SpeakerActor->StopAnimMontage();
		}
		
		StopAudio();

		if (BarkData.bNotifyLevelOnEnd)
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
			{
				if (const auto LevelScriptActor = GameInstance->GetLevelScriptActor())
				{
					LevelScriptActor->NotifyTag(BarkData.NotifyTag);
				}
			}
		}
	} else
	{
		CurrentDialogueLineIdx++;
		DisplayDialogueLine();
	}
}

void UUpPersistentBarkDisplayWidget::HandleBarkBroadcast(const FUpBarkData& InBarkData)
{
	bShow = false;
	CurrentDialogueLineIdx = 0;
	bMontagePlayed = false;

	BarkData = InBarkData;
	SpeakerName = FText::GetEmpty();

	// TODO(P1): Populate.
	SpeakerActor = nullptr;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto CharacterData = GameInstance->GetCharacterData(BarkData.SpeakerTagId); CharacterData.IsValid())
		{
			DialogueVoice = CharacterData.DialogueVoice;
			SpeakerName = CharacterData.Name;
		}
	}

	DisplayDialogueLine();
}

void UUpPersistentBarkDisplayWidget::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!BarkData.DialogueLines.IsValidIndex(CurrentDialogueLineIdx)) return;

	const auto& DialogueLine = BarkData.DialogueLines[CurrentDialogueLineIdx];

	if (!DialogueLine.MontageData.IsValidIndex(CurrentMontageIdx + 1)) return;

	CurrentMontageIdx++;
	
	if (const auto MontageDatum = DialogueLine.MontageData[CurrentMontageIdx]; MontageDatum.IsValid())
	{
		if (SpeakerActor) SpeakerActor->PlayAnimMontage(MontageDatum.Montage, MontageDatum.Rate, MontageDatum.StartSection);
	}
}

void UUpPersistentBarkDisplayWidget::DisplayDialogueLine()
{
	FDialogueContext DialogueContext;
	DialogueContext.Speaker = DialogueVoice;
		
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto PlayerDialogueVoice = GameInstance->GetPlayerDialogueVoice())
		{
			DialogueContext.Targets = TArray { PlayerDialogueVoice };
		}
	}
	
	if (BarkData.DialogueLines.IsValidIndex(CurrentDialogueLineIdx))
	{
		const auto& DialogueLine = BarkData.DialogueLines[CurrentDialogueLineIdx];

		if (const auto Widget = GetTextWidget())
		{
			FString WidgetText = SpeakerName.IsEmpty() ? TEXT("") : FString::Printf(TEXT("<RichText.Label>%s:</> "), *SpeakerName.ToString());

			WidgetText += UUpBlueprintFunctionLibrary::GetInGameNameifiedText(this,
				FText::FromString(DialogueLine.DialogueWave->SpokenText)).ToString();
			
			Widget->SetText(FText::FromString(WidgetText));
		}
		
		if (const auto DialogueWave = DialogueLine.DialogueWave;
			DialogueWave && DialogueContext.Speaker && DialogueContext.Targets.Num() > 0)
		{
			// Set volume to 0 since we're using placeholder audio in this prototype.
			CurrentDialogueLineAudio = UGameplayStatics::SpawnDialogue2D(this, DialogueWave, DialogueContext, 0.f);

			if (CurrentDialogueLineAudio) CurrentDialogueLineAudio->OnAudioFinished.AddDynamic(this, &ThisClass::AdvanceDialogueLine);
		}

		if (DialogueLine.MontageData.IsValidIndex(0) && DialogueLine.MontageData[0].IsValid() && SpeakerActor)
		{
			const auto MontageDatum = DialogueLine.MontageData[0];

			SpeakerActor->PlayAnimMontage(MontageDatum.Montage, MontageDatum.Rate, MontageDatum.StartSection);
			CurrentMontageIdx = 0;
			bMontagePlayed = true;
			
			if (const auto Mesh = SpeakerActor->GetMesh())
			{
				if (const auto AnimInstance = Mesh->GetAnimInstance();
					AnimInstance && !AnimInstance->OnMontageEnded.Contains(OnMontageEndedDelegate))
				{
					AnimInstance->OnMontageEnded.Add(OnMontageEndedDelegate);
				}
			}
		} else
		{
			if (bMontagePlayed && SpeakerActor) SpeakerActor->StopAnimMontage();

			bMontagePlayed = false;
		}
		
		bShow = true;
	}
}

void UUpPersistentBarkDisplayWidget::StopAudio() const
{
	if (IsValid(CurrentDialogueLineAudio)) CurrentDialogueLineAudio->Stop();
}
