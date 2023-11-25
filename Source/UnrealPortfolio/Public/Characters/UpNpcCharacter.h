// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/UpCharacter.h"
#include "Engine/DataTable.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpNpcCharacter.generated.h"

class AUpAiController;
class UBehaviorTree;
class UDialogueVoice;
class USphereComponent;
class UUpDialogueComponent;
class UUpGameInstance;

USTRUCT()
struct FUpNpcData : public FTableRowBase
{
	GENERATED_BODY()

	bool IsValid() const { return TagId.IsValid() && !Name.IsEmpty(); }

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly);
	FText Name;
};

UCLASS()
class UNREALPORTFOLIO_API AUpNpcCharacter : public AUpCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface,
	public IUpInteractable, public IUpNameable, public IUpTagIdable, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	static bool GrantTagSpec(UUpGameInstance* GameInstance, const FGameplayTag& NpcTagId, const FUpTagSpec& TagSpec);
	
	explicit AUpNpcCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual bool CanInteract() const override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override;

	virtual FGameplayTag GetTagId() const override { return TagId; }

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	void JumpToLocation(const FVector& TargetLocation, const float Duration);
	void ToggleSprint(const bool bSprint) const;

	FORCEINLINE UUpDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }
	FORCEINLINE UDialogueVoice* GetDialogueVoice() const { return DialogueVoice; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpDialogueComponent> DialogueComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDialogueVoice> DialogueVoice;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float InteractionSphereRadius = 100.f;

	UPROPERTY(Transient)
	TObjectPtr<AUpAiController> AiController;

	FUpNpcData NpcData;
};
