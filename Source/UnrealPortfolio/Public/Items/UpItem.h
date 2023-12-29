// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/UpInventoryComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "UpItem.generated.h"

class USphereComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpItem : public AStaticMeshActor, public IUpInteractable, public IUpNameable, public IUpTagIdable
{
	GENERATED_BODY()

public:
	AUpItem();
	
	virtual bool CanInteract() const override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return ItemData.Name; }

	virtual FGameplayTag GetTagId() const override { return TagId; }

	void AttachToComponentWithScaling(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, const FName& SocketName = NAME_None);
	void Detach();
	void ToggleCastShadows(const bool bEnable) const;

	FORCEINLINE FUpItemData GetItemData() const { return ItemData; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float InteractionSphereRadius = 50.f;

	FUpItemData ItemData;
	
	virtual void BeginPlay() override;
};
