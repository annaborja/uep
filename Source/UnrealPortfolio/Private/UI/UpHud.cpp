// Copyright AB. All Rights Reserved.

#include "UI/UpHud.h"

#include "Characters/UpNpcCharacter.h"
#include "Components/UpDialogueComponent.h"
#include "Components/UpReputationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UI/Persistent/UpPersistentOverlayWidget.h"
#include "UI/Dialogue/UpDialogueOverlayWidget.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"

int32 AUpHud::GetPlayerKarma() const
{
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		return GameMode->GetPlayerKarma();
	}

	return -1;
}

FUpRelationshipData AUpHud::GetPlayerRelationshipData() const
{
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		return GameMode->GetPlayerRelationshipData();
	}

	return FUpRelationshipData();
}

void AUpHud::Init(AUpPlayerController* InPlayerController)
{
	check(DialogueOverlayClass);
	check(MenuSwitcherClass);
	check(PersistentOverlayClass);
	
	CustomController = InPlayerController;

	// Initialize the persistent overlay.
	if (const auto World = GetWorld(); World && PersistentOverlayClass)
	{
		PersistentOverlayWidget = CreateWidget<UUpPersistentOverlayWidget>(World, PersistentOverlayClass);
		PersistentOverlayWidget->SetCustomHud(this);
		PersistentOverlayWidget->AddToViewport();
	}
}

void AUpHud::OpenMainMenu() const
{
	if (!PersistentOverlayWidget) return;
	
	PersistentOverlayWidget->OpenMainMenu();
}

void AUpHud::OpenDialogueFlow()
{
	if (!PersistentOverlayWidget) return;
	
	DialogueOverlayWidget = PersistentOverlayWidget->OpenDialogueFlow();
}

void AUpHud::CloseDialogueFlow() const
{
	if (!DialogueOverlayWidget) return;
	
	DialogueOverlayWidget->DeactivateWidget();
}

void AUpHud::DisplayDialogueStep(AUpNpcCharacter* Npc, const FUpDialogueStepData& DialogueStep) const
{
	if (!DialogueOverlayWidget) return;
	
	DialogueOverlayWidget->DisplayDialogueStep(Npc, DialogueStep);
}

void AUpHud::AdvanceDialogueStep(const AUpNpcCharacter* Npc, const FUpDialogueStepData& DialogueStep) const
{
	if (!CustomController) return;

	if (const auto DialogueComponent = Npc->GetDialogueComponent())
	{
		DialogueComponent->AdvanceDialogueStep(DialogueStep, CustomController);
	}
}

void AUpHud::DisplayDialogueOptions(AUpNpcCharacter* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions) const
{
	if (!DialogueOverlayWidget) return;
	
	DialogueOverlayWidget->DisplayDialogueOptions(Npc, DialogueOptions);
}

void AUpHud::SelectDialogueOption(const AUpNpcCharacter* Npc, const FUpDialogueOptionData& DialogueOption) const
{
	if (!CustomController) return;

	if (const auto DialogueComponent = Npc->GetDialogueComponent())
	{
		DialogueComponent->SelectDialogueOption(DialogueOption, CustomController);
	}
}

void AUpHud::BroadcastTargetInteractable(const AActor* TargetInteractable) const
{
	TargetInteractableDelegate.Broadcast(TargetInteractable);
}
