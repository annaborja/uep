// Copyright AB. All Rights Reserved.

#include "UI/UpHud.h"

#include "UpGameInstance.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
#include "Components/UpDialogueComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UI/CharacterSwitcher/UpCharacterSwitcherWidget.h"
#include "UI/Persistent/UpPersistentOverlayWidget.h"
#include "UI/Dialogue/UpDialogueOverlayWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void AUpHud::Init(AUpPlayerController* InPlayerController)
{
	check(CharacterSwitcherClass);
	check(DialogueOverlayClass);
	check(MenuSwitcherClass);
	check(PersistentOverlayClass);
	
	CustomController = InPlayerController;
	
	// Bind to GAS delegates.
	if (CustomController)
	{
		if (const auto PlayerState = CustomController->GetPlayerState<AUpPlayerState>())
		{
			if (const auto AbilitySystemComponent = Cast<UUpAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent()))
			{
				for (const auto AttributeSet : PlayerState->GetAttributeSets())
				{
					for (const auto TagAttributeMapping : AttributeSet->GetTagAttributeMap())
					{
						AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TagAttributeMapping.Value())
							.AddLambda([this, AttributeSet, TagAttributeMapping](const FOnAttributeChangeData& Data)
							{
								BroadcastAttributeValue(TagAttributeMapping.Key, TagAttributeMapping.Value(), AttributeSet);
							});
					}
				}	
			}
		}
	}

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

FUpReputationData AUpHud::GetPlayerWorldReputationData() const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		return GameInstance->GetPlayerWorldReputationData();
	}

	return FUpReputationData();
}

TMap<FGameplayTag, FUpReputationData> AUpHud::GetPlayerNpcReputationDataMap() const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		return GameInstance->GetPlayerNpcReputationDataMap();
	}

	return TMap<FGameplayTag, FUpReputationData>();
}

bool AUpHud::OpenCharacterSwitcher()
{
	if (!PersistentOverlayWidget) return false;
	
	// `OpenCharacterSwitcher()` may be triggered continuously through player input.
	if (CharacterSwitcherWidget && CharacterSwitcherWidget->IsActivated()) return false;
	
	CharacterSwitcherWidget = PersistentOverlayWidget->OpenCharacterSwitcher();
	
	return true;
}

void AUpHud::CloseCharacterSwitcher() const
{
	if (!CharacterSwitcherWidget) return;
	
	CharacterSwitcherWidget->DeactivateWidget();
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

void AUpHud::BroadcastAttributeValue(const FGameplayTag& Tag, const FGameplayAttribute& Attribute, const UUpAttributeSet* AttributeSet) const
{
	AttributeValueDelegate.Broadcast(Tag, Attribute.GetNumericValue(AttributeSet));
}
