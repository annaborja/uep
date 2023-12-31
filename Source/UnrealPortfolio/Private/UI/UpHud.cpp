// Copyright AB. All Rights Reserved.

#include "UI/UpHud.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/UpDialogueComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UI/CharacterSwitcher/UpCharacterSwitcherWidget.h"
#include "UI/Persistent/UpPersistentOverlayWidget.h"
#include "UI/Dialogue/UpDialogueOverlayWidget.h"

void AUpHud::Init(AUpPlayerController* InPlayerController)
{
	check(CharacterSwitcherClass);
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

void AUpHud::OpenCharacterSwitcher()
{
	if (!PersistentOverlayWidget) return;

	if (CharacterSwitcherWidget && CharacterSwitcherWidget->IsActivated())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character switcher already open"))
		return;
	}
	
	CharacterSwitcherWidget = PersistentOverlayWidget->OpenCharacterSwitcher();
}

void AUpHud::CloseCharacterSwitcher() const
{
	if (!CharacterSwitcherWidget) return;
	
	CharacterSwitcherWidget->DeactivateWidget();
}

void AUpHud::SwitchCharacter(AUpPlayableNpc* Npc) const
{
	if (!CustomController) return;

	CustomController->SwitchCharacter(Npc);
}

void AUpHud::OpenDialogueFlow()
{
	if (!PersistentOverlayWidget) return;
	
	if (DialogueOverlayWidget && DialogueOverlayWidget->IsActivated())
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue flow already open"))
		return;
	}
	
	DialogueOverlayWidget = PersistentOverlayWidget->OpenDialogueFlow();
}

void AUpHud::CloseDialogueFlow() const
{
	if (!DialogueOverlayWidget) return;
	
	DialogueOverlayWidget->DeactivateWidget();
}

void AUpHud::DisplayDialogueStep(AUpPlayableNpc* Npc, const FUpDialogueStepData& DialogueStep) const
{
	if (!DialogueOverlayWidget) return;
	
	DialogueOverlayWidget->DisplayDialogueStep(Npc, DialogueStep);
}

void AUpHud::AdvanceDialogueStep(const AUpPlayableNpc* Npc, const FUpDialogueStepData& DialogueStep) const
{
	if (!CustomController) return;

	if (const auto DialogueComponent = Npc->GetDialogueComponent())
	{
		DialogueComponent->AdvanceDialogueStep(DialogueStep, CustomController);
	}
}

void AUpHud::DisplayDialogueOptions(AUpPlayableNpc* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions) const
{
	if (!DialogueOverlayWidget) return;
	
	DialogueOverlayWidget->DisplayDialogueOptions(Npc, DialogueOptions);
}

void AUpHud::SelectDialogueOption(const AUpPlayableNpc* Npc, const FUpDialogueOptionData& DialogueOption) const
{
	if (!CustomController) return;

	if (const auto DialogueComponent = Npc->GetDialogueComponent())
	{
		DialogueComponent->SelectDialogueOption(DialogueOption, CustomController);
	}
}

void AUpHud::BroadcastActiveWeapon(const AUpWeapon* Weapon) const
{
	ActiveWeaponDelegate.Broadcast(Weapon);
}

void AUpHud::BroadcastAttributeValue(const FGameplayTag& TagId, const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute, const UUpAttributeSet* AttributeSet) const
{
	AttributeValueDelegate.Broadcast(TagId, AttributeTag, Attribute.GetNumericValue(AttributeSet));
}

void AUpHud::BroadcastBark(const FUpBarkData& BarkData) const
{
	BarkDelegate.Broadcast(BarkData);
}

void AUpHud::BroadcastEquipmentActivationUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const bool bActivated) const
{
	EquipmentActivationUpdateDelegate.Broadcast(EquipmentSlot, bActivated);
}

void AUpHud::BroadcastEquipmentUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const FUpItemData& ItemData) const
{
	EquipmentUpdateDelegate.Broadcast(EquipmentSlot, ItemData);
}

void AUpHud::BroadcastInteractionData(const FUpInteractionData InteractionData) const
{
	InteractionDataDelegate.Broadcast(InteractionData);
}

void AUpHud::BroadcastPossessedCharacter(AUpPlayableCharacter* PossessedCharacter) const
{
	PossessedCharacterDelegate.Broadcast(PossessedCharacter);
}

void AUpHud::BroadcastSecondarySquadMember(AUpPlayableCharacter* Character) const
{
	SecondarySquadMemberDelegate.Broadcast(Character);
}

void AUpHud::BroadcastTutorial(const FUpTutorialData& TutorialData) const
{
	TutorialDelegate.Broadcast(TutorialData);
}
