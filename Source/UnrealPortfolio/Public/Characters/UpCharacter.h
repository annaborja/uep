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

class AUpWeapon;
struct FUpItemData;
class UGameplayEffect;
class UUpAmmoAttributeSet;
class UUpAttributeSet;
class UUpCharacterMovementComponent;
class UUpPrimaryAttributeSet;
class UUpVitalAttributeSet;

USTRUCT(BlueprintType)
struct FUpCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	bool IsValid() const { return TagId.IsValid() && !Name.IsEmpty(); }

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TagId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_FullBody;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image_Head;
};

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
	bool ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);
	bool DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);

	FORCEINLINE FUpCharacterData GetCharacterData() const { return CharacterData; }
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
	TSubclassOf<UGameplayEffect> InitAmmoAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;
	
	UPROPERTY(EditAnywhere, Category="UP Data")
	FUpCharacterEquipment Equipment;

	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
	UPROPERTY(Transient)
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;
	UPROPERTY(Transient)
	TObjectPtr<AUpWeapon> WeaponActor;

	UPROPERTY()
	TObjectPtr<UUpAmmoAttributeSet> AmmoAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpVitalAttributeSet> VitalAttributeSet;
	
	FUpCharacterData CharacterData;
	
	FVector RootMotionTargetLocation;
	bool bHasRootMotionTargetLocation = false;

	virtual void OnEquipmentActivation(const EUpEquipmentSlot::Type EquipmentSlot) {}
	virtual void OnEquipmentDeactivation(const EUpEquipmentSlot::Type EquipmentSlot) {}
};
