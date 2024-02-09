// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/UpPlayableCharacter.h"
#include "Components/UpDialogueComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/HUD.h"
#include "Items/UpWeapon.h"
#include "UpHud.generated.h"

class AUpPlayableCharacter;
class AUpPlayableNpc;
class AUpPlayerController;
struct FGameplayAttribute;
struct FUpBarkData;
struct FUpDialogueLine;
struct FUpDialogueOptionData;
struct FUpDialogueStepData;
class UUpAttributeSet;
class UUpCharacterSwitcherWidget;
class UUpCommonActivatableWidget;
class UUpDialogueOverlayWidget;
class UUpPersistentOverlayWidget;

USTRUCT()
struct FUpMenuTabData : public FTableRowBase
{
	GENERATED_BODY();

	bool IsValid() const { return TagId.IsValid() && !Label.IsEmpty(); }
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;

	UPROPERTY(EditDefaultsOnly)
	FText Label;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudActiveWeaponSignature, const AUpWeapon*);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FUpHudAttributeValueSignature, const FGameplayTag&, const FGameplayTag&, const float);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudBarkSignature, const FUpBarkData& BarkData);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpHudEquipmentActivationUpdateSignature, const EUpEquipmentSlot::Type, const bool bActivated);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpHudEquipmentUpdateSignature, const EUpEquipmentSlot::Type, const FUpItemData& ItemData);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudInteractionDataSignature, const FUpInteractionData);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudPossessedCharacterSignature, AUpPlayableCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudSecondarySquadMemberSignature, AUpPlayableCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudTutorialSignature, const FUpTutorialData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudObjectiveSignature, const FUpObjectiveData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FUpHudNotificationSignature, const FUpNotificationData&);

UCLASS()
class UNREALPORTFOLIO_API AUpHud : public AHUD
{
	GENERATED_BODY()

public:
	FUpHudActiveWeaponSignature ActiveWeaponDelegate;
	FUpHudAttributeValueSignature AttributeValueDelegate;
	FUpHudBarkSignature BarkDelegate;
	FUpHudEquipmentActivationUpdateSignature EquipmentActivationUpdateDelegate;
	FUpHudEquipmentUpdateSignature EquipmentUpdateDelegate;
	FUpHudInteractionDataSignature InteractionDataDelegate;
	FUpHudPossessedCharacterSignature PossessedCharacterDelegate;
	FUpHudSecondarySquadMemberSignature SecondarySquadMemberDelegate;
	FUpHudTutorialSignature TutorialDelegate;
	FUpHudObjectiveSignature ObjectiveDelegate;
	FUpHudNotificationSignature NotificationDelegate;
	
	void Init(AUpPlayerController* InPlayerController);
	void OpenMainMenu() const;
	bool IsMainMenuOpen() const;
	
	void OpenCharacterSwitcher();
	void CloseCharacterSwitcher() const;
	void SwitchCharacter(AUpPlayableNpc* Npc) const;
	
	void OpenDialogueFlow();
	void CloseDialogueFlow() const;
	void DisplayDialogueStep(AUpPlayableNpc* Npc, const FUpDialogueStepData& DialogueStep) const;
	void AdvanceDialogueStep(const AUpPlayableNpc* Npc, const FUpDialogueStepData& DialogueStep) const;
	void DisplayDialogueOptions(AUpPlayableNpc* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions) const;
	void SelectDialogueOption(const AUpPlayableNpc* Npc, const FUpDialogueOptionData& DialogueOption) const;

	void BroadcastActiveWeapon(const AUpWeapon* Weapon) const;
	void BroadcastAttributeValue(const FGameplayTag& TagId, const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute, const UUpAttributeSet* AttributeSet) const;
	void BroadcastBark(const FUpBarkData& BarkData) const;
	void BroadcastEquipmentActivationUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const bool bActivated) const;
	void BroadcastEquipmentUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const FUpItemData& ItemData) const;
	void BroadcastInteractionData(const FUpInteractionData InteractionData) const;
	void BroadcastPossessedCharacter(AUpPlayableCharacter* PossessedCharacter) const;
	void BroadcastSecondarySquadMember(AUpPlayableCharacter* Character) const;
	void BroadcastTutorial(const FUpTutorialData& TutorialData) const;
	void BroadcastObjective(const FUpObjectiveData& ObjectiveData) const;
	void BroadcastNotification(const FUpNotificationData& NotificationData) const;

	FORCEINLINE AUpPlayerController* GetCustomController() const { return CustomController; }
	
	FORCEINLINE TSubclassOf<UUpCharacterSwitcherWidget> GetCharacterSwitcherClass() const { return CharacterSwitcherClass; }
	FORCEINLINE TSubclassOf<UUpDialogueOverlayWidget> GetDialogueOverlayClass() const { return DialogueOverlayClass; }
	FORCEINLINE TSubclassOf<UUpCommonActivatableWidget> GetMenuSwitcherClass() const { return MenuSwitcherClass; }
	
	FORCEINLINE UUpCharacterSwitcherWidget* GetCharacterSwitcher() const { return CharacterSwitcherWidget; }
	FORCEINLINE UUpPersistentOverlayWidget* GetPersistentOverlay() const { return PersistentOverlayWidget; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpCharacterSwitcherWidget> CharacterSwitcherClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpDialogueOverlayWidget> DialogueOverlayClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpCommonActivatableWidget> MenuSwitcherClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpPersistentOverlayWidget> PersistentOverlayClass;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
	
	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterSwitcherWidget> CharacterSwitcherWidget;
	UPROPERTY(Transient)
	TObjectPtr<UUpDialogueOverlayWidget> DialogueOverlayWidget;
	UPROPERTY(Transient)
	TObjectPtr<UUpPersistentOverlayWidget> PersistentOverlayWidget;
};
