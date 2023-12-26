// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Components/UpInventoryComponent.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/UpCombatable.h"
#include "Utils/Enums.h"
#include "UpCharacter.generated.h"

class AStaticMeshActor;
struct FUpItemData;
class UGameplayEffect;
class UUpAttributeSet;
class UUpCharacterMovementComponent;
class UUpPrimaryAttributeSet;
class UUpVitalAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter, public IAbilitySystemInterface, public IUpCombatable
{
	GENERATED_BODY()

public:
	AUpCharacter();
	explicit AUpCharacter(const FObjectInitializer& ObjectInitializer) : AUpCharacter() {}

	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual void Die() override;
	virtual UUpCombatComponent* GetCombatComponent() const override { return CombatComponent; }
	virtual UAnimMontage* GetHitReactionsMontage() const override { return HitReactionsMontage_ThirdPerson; }

	void SetYaw(const float InYaw);
	
	void SetRootMotionTargetLocation(const FVector& InRootMotionTargetLocation);
	void UnsetRootMotionTargetLocation();
	
	virtual EUpCameraView::Type GetCameraView() const { return EUpCameraView::ThirdPerson; }
	
	virtual TArray<UUpAttributeSet*> GetAttributeSets() const;
	
	FUpCharacterEquipment GetCharacterEquipment() const { return Equipment; }
	void ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot, const FGameplayTag& ItemTagId);
	void DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);
	
	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	FORCEINLINE EUpCharacterPosture::Type GetPosture() const { return Posture; }
	
	FORCEINLINE FVector GetRootMotionTargetLocation() const { return RootMotionTargetLocation; }
	FORCEINLINE bool HasRootMotionTargetLocation() const { return bHasRootMotionTargetLocation; }
	
	FORCEINLINE UUpPrimaryAttributeSet* GetPrimaryAttributeSet() const { return PrimaryAttributeSet; }
	FORCEINLINE UUpVitalAttributeSet* GetVitalAttributeSet() const { return VitalAttributeSet; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpCombatComponent> CombatComponent;
	
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TObjectPtr<UAnimMontage> HitReactionsMontage_ThirdPerson;
	
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitHealthAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
	
	UPROPERTY(EditAnywhere, Category="UP Data")
	FUpCharacterEquipment Equipment;

	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
	UPROPERTY(Transient)
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;
	UPROPERTY(Transient)
	TObjectPtr<AStaticMeshActor> WeaponActor;

	UPROPERTY()
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpVitalAttributeSet> VitalAttributeSet;
	
	FVector RootMotionTargetLocation;
	bool bHasRootMotionTargetLocation = false;

	virtual void OnEquipmentActivation(const FUpItemData& ItemData) {}
	virtual void OnEquipmentDeactivation(const FUpItemData& ItemData) {}
};
