// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/UpInventoryComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Interfaces/UpInteractable.h"
#include "Interfaces/UpNameable.h"
#include "Interfaces/UpTagIdable.h"
#include "UpWeapon.generated.h"

class USphereComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpWeapon : public AStaticMeshActor, public IUpInteractable, public IUpNameable, public IUpTagIdable
{
	GENERATED_BODY()

public:
	AUpWeapon();
	
	virtual bool CanInteract() const override;
	virtual void Interact(AUpPlayerController* PlayerController) override;

	virtual FText GetInGameName() const override { return ItemData.Name; }

	virtual FGameplayTag GetTagId() const override { return TagId; }
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractionSphere;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float InteractionSphereRadius = 50.f;

	FUpItemData ItemData;
};
