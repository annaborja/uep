// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/UpCharacter.h"
#include "Engine/DataTable.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpNpcCharacter.generated.h"

class AUnrealPortfolioGameModeBase;
class UDialogueVoice;
class USphereComponent;
class UUpDialogueComponent;

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
class UNREALPORTFOLIO_API AUpNpcCharacter : public AUpCharacter, public IGameplayTagAssetInterface,
	public IUpInteractable, public IUpNameable, public IUpTagIdable, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	static bool GrantTagSpec(AUnrealPortfolioGameModeBase* GameMode, const FGameplayTag& NpcTagId, const FUpTagSpec& TagSpec);
	
	explicit AUpNpcCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual bool CanInteract() const override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override;

	virtual FGameplayTag GetTagId() const override { return TagId; }

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	FORCEINLINE UUpDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }
	FORCEINLINE UDialogueVoice* GetDialogueVoice() const { return DialogueVoice; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpDialogueComponent> DialogueComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UDialogueVoice> DialogueVoice;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float InteractionSphereRadius = 100.f;

	FUpNpcData NpcData;
};
