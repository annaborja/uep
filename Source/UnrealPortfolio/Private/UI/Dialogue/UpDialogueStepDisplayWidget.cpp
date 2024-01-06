// Copyright AB. All Rights Reserved.

#include "UI/Dialogue/UpDialogueStepDisplayWidget.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/DialogueWave.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpDialogueStepDisplayWidget::DisplayDialogueStep(AUpPlayableNpc* InNpc, const FUpDialogueStepData& InDialogueStep)
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

void UUpDialogueStepDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	OnMontageEndedDelegate.BindUFunction(this, FName("HandleMontageEnded"));
}

void UUpDialogueStepDisplayWidget::AdvanceDialogueLine()
{
	if (CurrentDialogueLineIndex >= DialogueStep.DialogueLines.Num() - 1)
	{
		if (const auto NpcMesh = Npc->GetMesh())
		{
			if (const auto NpcAnimInstance = NpcMesh->GetAnimInstance();
				NpcAnimInstance && NpcAnimInstance->OnMontageEnded.Contains(OnMontageEndedDelegate))
			{
				NpcAnimInstance->OnMontageEnded.Remove(OnMontageEndedDelegate);
			}
		}
		
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

void UUpDialogueStepDisplayWidget::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!DialogueStep.DialogueLines.IsValidIndex(CurrentDialogueLineIndex)) return;

	const auto DialogueLine = DialogueStep.DialogueLines[CurrentDialogueLineIndex];

	if (!DialogueLine.MontageData.IsValidIndex(CurrentMontageIndex + 1)) return;

	CurrentMontageIndex++;
	
	if (const auto MontageDatum = DialogueLine.MontageData[CurrentMontageIndex]; MontageDatum.IsValid())
	{
		Npc->PlayAnimMontage(MontageDatum.Montage, MontageDatum.Rate, MontageDatum.StartSection);
	}
}

void UUpDialogueStepDisplayWidget::DisplayDialogueLine()
{
	FDialogueContext DialogueContext;
	DialogueContext.Speaker = Npc->GetCharacterData().DialogueVoice;
		
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto PlayerDialogueVoice = GameInstance->GetPlayerDialogueVoice())
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

		if (DialogueLine.MontageData.IsValidIndex(0) && DialogueLine.MontageData[0].IsValid())
		{
			const auto MontageDatum = DialogueLine.MontageData[0];

			Npc->PlayAnimMontage(MontageDatum.Montage, MontageDatum.Rate, MontageDatum.StartSection);
			CurrentMontageIndex = 0;
			
			if (const auto NpcMesh = Npc->GetMesh())
			{
				if (const auto NpcAnimInstance = NpcMesh->GetAnimInstance();
					NpcAnimInstance && !NpcAnimInstance->OnMontageEnded.Contains(OnMontageEndedDelegate))
				{
					NpcAnimInstance->OnMontageEnded.Add(OnMontageEndedDelegate);
				}
			}
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
