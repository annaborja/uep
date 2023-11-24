// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "GameFramework/HUD.h"
#include "UpHud.generated.h"

class AUpNpcCharacter;
class AUpPlayerController;
struct FGameplayAttribute;
struct FUpDialogueOptionData;
struct FUpDialogueStepData;
struct FUpReputationData;
class UUpAttributeSet;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpHudAttributeValueSignature, const FGameplayTag&, Tag, const float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpHudTargetInteractableSignature, const AActor*, TargetInteractable);

UCLASS()
class UNREALPORTFOLIO_API AUpHud : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FUpHudAttributeValueSignature AttributeValueDelegate;
	
	void Init(AUpPlayerController* InPlayerController);
	
	void OpenMainMenu() const;
	FUpReputationData GetPlayerWorldReputationData() const;
	TMap<FGameplayTag, FUpReputationData> GetPlayerNpcReputationDataMap() const;
	
	// Dialogue Flow
	void OpenDialogueFlow();
	void CloseDialogueFlow() const;
	void DisplayDialogueStep(AUpNpcCharacter* Npc, const FUpDialogueStepData& DialogueStep) const;
	void AdvanceDialogueStep(const AUpNpcCharacter* Npc, const FUpDialogueStepData& DialogueStep) const;
	void DisplayDialogueOptions(AUpNpcCharacter* Npc, const TArray<FUpDialogueOptionData>& DialogueOptions) const;
	void SelectDialogueOption(const AUpNpcCharacter* Npc, const FUpDialogueOptionData& DialogueOption) const;

	// Delegate Broadcasts
	void BroadcastTargetInteractable(const AActor* TargetInteractable) const;

	FORCEINLINE AUpPlayerController* GetCustomController() const { return CustomController; }
	
	FORCEINLINE TSubclassOf<UUpDialogueOverlayWidget> GetDialogueOverlayClass() const { return DialogueOverlayClass; }
	FORCEINLINE TSubclassOf<UUpCommonActivatableWidget> GetMenuSwitcherClass() const { return MenuSwitcherClass; }
	
	FUpHudTargetInteractableSignature TargetInteractableDelegate;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpDialogueOverlayWidget> DialogueOverlayClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpCommonActivatableWidget> MenuSwitcherClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpPersistentOverlayWidget> PersistentOverlayClass;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
	UPROPERTY(Transient)
	TObjectPtr<UUpDialogueOverlayWidget> DialogueOverlayWidget;
	UPROPERTY(Transient)
	TObjectPtr<UUpPersistentOverlayWidget> PersistentOverlayWidget;

	void BroadcastAttributeValue(const FGameplayTag& Tag, const FGameplayAttribute& Attribute, const UUpAttributeSet* AttributeSet) const;
};
