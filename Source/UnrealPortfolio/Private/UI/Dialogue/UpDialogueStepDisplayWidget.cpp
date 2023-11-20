// Copyright AB. All Rights Reserved.

#include "UI/Dialogue/UpDialogueStepDisplayWidget.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/DialogueWave.h"
#include "UI/UpHud.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpDialogueStepDisplayWidget::DisplayDialogueStep(AUpNpcCharacter* InNpc, const FUpDialogueStepData& InDialogueStep)
{
	CurrentDialogueLineIndex = 0;
	DialogueStep = InDialogueStep;
	Npc = InNpc;

	DisplayDialogueLine();
}

void UUpDialogueStepDisplayWidget::HandleContinueAction() const
{
	StopAudio();
}

void UUpDialogueStepDisplayWidget::AdvanceDialogueLine()
{
	if (CurrentDialogueLineIndex >= DialogueStep.DialogueLines.Num() - 1)
	{
		Npc->StopAnimMontage();
		StopAudio();

		if (IsValid(CustomHud))
		{
			CustomHud->AdvanceDialogueStep(Npc, DialogueStep);
		}
	} else
	{
		CurrentDialogueLineIndex++;
		DisplayDialogueLine();
	}
}

void UUpDialogueStepDisplayWidget::DisplayDialogueLine()
{
	FDialogueContext DialogueContext;
	DialogueContext.Speaker = Npc->GetDialogueVoice();
		
	if (const auto GameMode = UUpBlueprintFunctionLibrary::GetGameMode<AUnrealPortfolioGameModeBase>(this))
	{
		if (const auto PlayerDialogueVoice = GameMode->GetPlayerDialogueVoice())
		{
			DialogueContext.Targets = TArray { PlayerDialogueVoice };
		}
	}
	
	if (DialogueStep.DialogueLines.IsValidIndex(CurrentDialogueLineIndex))
	{
		const auto DialogueLine = DialogueStep.DialogueLines[CurrentDialogueLineIndex];
		const auto SpokenText = FText::FromString(DialogueLine.DialogueWave->SpokenText);
		
		DialogueLineText = UUpBlueprintFunctionLibrary::GetInGameNameifiedText(this, SpokenText);
		NpcName = Npc->GetInGameName();

		if (const auto DialogueWave = DialogueLine.DialogueWave;
			DialogueWave && DialogueContext.Speaker && DialogueContext.Targets.Num() > 0)
		{
			// Set volume to 0 since we're using placeholder audio in this prototype.
			CurrentDialogueLineAudio = UGameplayStatics::SpawnDialogue2D(this, DialogueWave, DialogueContext, 0.f);

			if (CurrentDialogueLineAudio)
			{
				CurrentDialogueLineAudio->OnAudioFinished.AddDynamic(this, &ThisClass::AdvanceDialogueLine);
			}
		}

		if (const auto Montage = DialogueLine.MontageData.Montage)
		{
			Npc->PlayAnimMontage(Montage, DialogueLine.MontageData.Rate, DialogueLine.MontageData.StartSection);
		} else
		{
			Npc->StopAnimMontage();
		}
	}
}

void UUpDialogueStepDisplayWidget::StopAudio() const
{
	if (IsValid(CurrentDialogueLineAudio))
	{
		CurrentDialogueLineAudio->Stop();
	}
}
