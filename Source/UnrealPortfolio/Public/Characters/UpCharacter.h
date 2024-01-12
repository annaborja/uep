// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/UpCombatable.h"
#include "Interfaces/UpTagIdable.h"
#include "Utils/Structs.h"
#include "UpCharacter.generated.h"

class UBehaviorTree;
class UDialogueVoice;
class UGameplayEffect;
class USoundCue;
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDialogueVoice> DialogueVoice;
};

UCLASS()
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter, public IAbilitySystemInterface, public IUpCombatable, public IUpTagIdable
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
	virtual UAnimMontage* GetReloadsMontage() const override { return ReloadsMontage_ThirdPerson; }
	
	virtual FGameplayTag GetTagId() const override { return TagId; }

	FRotator GetSafeRotation() const;
	void SetYaw(const float InYaw);
	
	void SetRootMotionTargetLocation(const FVector& InRootMotionTargetLocation);
	void UnsetRootMotionTargetLocation();
	
	virtual EUpCameraView::Type GetCameraView() const { return EUpCameraView::ThirdPerson; }
	bool IsInFirstPersonCameraView() const;
	void SetRelaxed(const bool bInRelaxed);
	
	virtual TArray<UUpAttributeSet*> GetAttributeSets() const;
	
	FUpCharacterEquipment GetCharacterEquipment() const { return Equipment; }
	void EquipItem(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot);
	void UnequipItem(const EUpEquipmentSlot::Type EquipmentSlot);
	bool ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);
	bool DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);

	virtual UAnimMontage* GetGesturesMontage() const { return GesturesMontage_ThirdPerson; }
	virtual UAnimMontage* GetMantlesMontage() const { return MantlesMontage_ThirdPerson; }

	void HandleFootstep(const FName& BoneName, const EUpTraceDirection::Type TraceDirection = EUpTraceDirection::Down,
		const float TraceLength = 10.f, const float VolumeMultiplier = 1.f) const;
	void HandleJumpLaunch() const;
	void HandleLanding(const FName& BoneName, const EUpTraceDirection::Type TraceDirection = EUpTraceDirection::Down,
		const float TraceLength = 10.f, const float VolumeMultiplier = 1.f) const;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE FUpCharacterData GetCharacterData() const { return CharacterData; }
	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	FORCEINLINE EUpCharacterPosture::Type GetPosture() const { return Posture; }
	FORCEINLINE bool IsRelaxed() const { return bRelaxed; }
	
	FORCEINLINE FVector GetRootMotionTargetLocation() const { return RootMotionTargetLocation; }
	FORCEINLINE bool HasRootMotionTargetLocation() const { return bHasRootMotionTargetLocation; }
	
	FORCEINLINE UUpPrimaryAttributeSet* GetPrimaryAttributeSet() const { return PrimaryAttributeSet; }
	FORCEINLINE UUpVitalAttributeSet* GetVitalAttributeSet() const { return VitalAttributeSet; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpCombatComponent> CombatComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> GesturesMontage_ThirdPerson;
	UPROPERTY(EditAnywhere, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> HitReactionsMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> MantlesMontage_ThirdPerson;
	UPROPERTY(EditAnywhere, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> ReloadsMontage_ThirdPerson;
	
	UPROPERTY(EditAnywhere, Category="UP Assets|GAS")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributesEffectClass;
	UPROPERTY(EditAnywhere, Category="UP Assets|GAS")
	TSubclassOf<UGameplayEffect> InitVitalAttributesEffectClass;
	
	UPROPERTY(EditAnywhere, Category="UP Assets|SFX")
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap_Footsteps;
	UPROPERTY(EditAnywhere, Category="UP Assets|SFX")
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap_JumpLandings;
	UPROPERTY(EditAnywhere, Category="UP Assets|SFX")
	TObjectPtr<USoundCue> Sfx_JumpLaunches;
	
	UPROPERTY(EditAnywhere, Category="UP Data")
	FUpCharacterEquipment Equipment;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugPhysicalMaterials = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	
	UPROPERTY(Transient, VisibleAnywhere, Category="UP Runtime")
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;
	UPROPERTY(VisibleAnywhere, Category="UP Runtime")
	bool bRelaxed = true;
	
	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;

	UPROPERTY()
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpVitalAttributeSet> VitalAttributeSet;
	
	FUpCharacterData CharacterData;
	
	FVector RootMotionTargetLocation;
	bool bHasRootMotionTargetLocation = false;

	AUpItem* SpawnAndAttachItem(const TSubclassOf<AUpItem> ItemClass);
	
	void AttachAndHideItem(AUpItem* ItemActor);
	void AttachAndShowItem(AUpItem* ItemActor, const FName& SocketName) const;

	void HandleNoise(const TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap, const FName& BoneName,
		const EUpTraceDirection::Type TraceDirection = EUpTraceDirection::Down, const float TraceLength = 10.f, const float VolumeMultiplier = 1.f) const;
	
	virtual void OnItemEquip(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot) {}
	virtual void OnItemUnequip(const EUpEquipmentSlot::Type EquipmentSlot) {}
	virtual void OnEquipmentActivation(const EUpEquipmentSlot::Type EquipmentSlot) {}
	virtual void OnEquipmentDeactivation(const EUpEquipmentSlot::Type EquipmentSlot) {}
};
