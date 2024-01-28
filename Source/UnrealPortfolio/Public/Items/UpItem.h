// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/Player/UpPlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "Utils/Structs.h"
#include "UpItem.generated.h"

class USoundCue;
class USphereComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpItem : public AStaticMeshActor, public IUpInteractable, public IUpNameable, public IUpTagIdable
{
	GENERATED_BODY()

public:
	AUpItem();
	
	virtual FUpInteractionData GetInteractionData(const AUpPlayerController* PlayerController) override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return ItemData.Name; }

	virtual FGameplayTag GetTagId() const override { return TagId; }

	virtual EUpCharacterPosture::Type GetCharacterPosture() const { return EUpCharacterPosture::Casual; }
	
	void AttachToComponentWithScaling(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, const FName& SocketName = NAME_None);
	void Attach(AActor* Actor);
	void Detach();
	void ToggleCastShadows(const bool bEnable) const;

	FORCEINLINE FUpItemData GetItemData() const { return ItemData; }
	FORCEINLINE USoundCue* GetSfx_Equip() const { return Sfx_Equip; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TObjectPtr<USoundCue> Sfx_Equip;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebug = false;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag ItemDataTagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag RelatedTag;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	int32 Quantity = 1;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float RelatedQuantity = 0.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	bool bCanInteract = false;
	UPROPERTY(EditAnywhere, Category="UP Params")
	bool bDestroyOnInteract = false;

	FUpItemData ItemData;
	
	virtual void BeginPlay() override;

	virtual bool CanInteract(const AUpPlayerController* PlayerController) const { return bCanInteract; }
	virtual FText GetInteractionPromptText(const AUpPlayerController* PlayerController) const { return GetInGameName(); }
	virtual FText GetInteractionPromptSubText(const AUpPlayerController* PlayerController) const { return FText::GetEmpty(); }
	virtual int32 GetInteractionQuantity(const AUpPlayerController* PlayerController, const FGameplayTag& DynamicRelatedTag) const { return Quantity; }
	virtual float GetInteractionRelatedQuantity(const AUpPlayerController* PlayerController, const FGameplayTag& DynamicRelatedTag) const { return RelatedQuantity; }
	virtual FGameplayTag GetInteractionRelatedTag(const AUpPlayerController* PlayerController) const { return RelatedTag; }
};
