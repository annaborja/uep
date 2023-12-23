// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "Components/UpInventoryComponent.h"
#include "Interfaces/UpCharacterEquippable.h"
#include "UpNonPlayableNpc.generated.h"

class AUpAiController;
class UBehaviorTree;
class UUpAbilitySystemComponent;
class UUpCombatComponent;
class UUpHealthAttributeSet;
class UUpPrimaryAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API AUpNonPlayableNpc : public AUpCharacter, public IUpCharacterEquippable
{
	GENERATED_BODY()

public:
	explicit AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;

	virtual FUpCharacterEquipment GetCharacterEquipment() const override { return Equipment; }
	virtual void ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot, const FUpEquipmentSlotData& EquipmentSlotData) override;
	virtual void DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpCombatComponent> CombatComponent;

	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category="UP Data")
	FUpCharacterEquipment Equipment;

	UPROPERTY(Transient)
	TObjectPtr<AUpAiController> AiController;
	
	UPROPERTY()
	TObjectPtr<UUpHealthAttributeSet> HealthAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
};
