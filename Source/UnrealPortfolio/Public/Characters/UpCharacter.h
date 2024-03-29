// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/UpCombatable.h"
#include "Interfaces/UpTagIdable.h"
#include "Tags/GasTags.h"
#include "Utils/Structs.h"
#include "UpCharacter.generated.h"

class UUpCombatComponent;
class AUpWeapon;
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
class UNREALPORTFOLIO_API AUpCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IUpCombatable, public IUpTagIdable
{
	GENERATED_BODY()

public:
	AUpCharacter();
	explicit AUpCharacter(const FObjectInitializer& ObjectInitializer) : AUpCharacter() {}

	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual float CalculateDamageDealt(const FHitResult& HitResult) const override;
	virtual void Die() override;
	virtual UAnimMontage* GetHitReactionsMontage() const override { return HitReactionsMontage_ThirdPerson; }
	virtual UAnimMontage* GetReloadsMontage() const override { return ReloadsMontage_ThirdPerson; }
	
	void AddActiveHitBox(const FDataTableRowHandle& HitBoxDataRowHandle);
	void RemoveActiveHitBox(const FDataTableRowHandle& HitBoxDataRowHandle);
	void CleanUpCombatData();

	FORCEINLINE TArray<FName> GetActiveHitBoxNames() const { return ActiveHitBoxNames; }
	FORCEINLINE TArray<AActor*> GetHitActors() const { return HitActors; }

	virtual FGameplayTag GetTagId() const override { return TagId; }

	float GetHorizontalSpeed() const;
	float GetMovementOffsetYaw() const;
	void SetYaw(const float InYaw);
	
	void SetRootMotionTargetLocation(const FVector& InRootMotionTargetLocation);
	void UnsetRootMotionTargetLocation();
	
	virtual EUpCameraView::Type GetCameraView() const { return EUpCameraView::ThirdPerson; }
	bool IsAiming() const;
	bool IsBusy() const;
	bool IsDead() const;
	bool IsMeleeAttacking() const;
	bool IsInFirstPersonMode() const;
	bool IsInStrafingMode() const;
	bool IsShooting() const;
	bool IsStrafingForward() const;
	void SetPosture(const EUpCharacterPosture::Type InPosture) { Posture = InPosture; }
	void SetRelaxed(const bool bInRelaxed);
	void ToggleSprint(const bool bSprint) const;
	
	virtual TArray<UUpAttributeSet*> GetAttributeSets() const;

	void EquipItem(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot);
	void UnequipItem(const EUpEquipmentSlot::Type EquipmentSlot);
	bool ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);
	bool DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot);
	AUpWeapon* GetActiveWeapon() const;

	int8 GetNumShotsToTake() const;
	
	void AttachActivatedItem(AUpItem* ItemActor, const FName& SocketName = NAME_None);
	void AttachDeactivatedItem(AUpItem* ItemActor);

	virtual UAnimMontage* GetClimbingMontage() const { return ClimbingMontage_ThirdPerson; }
	virtual UAnimMontage* GetDeathsMontage() const { return DeathsMontage_ThirdPerson; }
	virtual UAnimMontage* GetGesturesMontage() const { return GesturesMontage_ThirdPerson; }
	virtual UAnimMontage* GetGunFiringMontage() const { return GunFiringMontage_ThirdPerson; }
	virtual UAnimMontage* GetGunMeleeAttacksMontage() const { return GunMeleeAttacksMontage_ThirdPerson; }
	virtual UAnimMontage* GetMantlesMontage() const { return MantlesMontage_ThirdPerson; }
	virtual UAnimMontage* GetSpecialMovesMontage() const { return SpecialMovesMontage_ThirdPerson; }
	virtual UAnimMontage* GetWeaponEquipMontage() const { return WeaponEquipMontage_ThirdPerson; }

	void HandleFootstep(const FName& BoneName, const EUpTraceDirection::Type TraceDirection = EUpTraceDirection::Down,
		const float TraceLength = 10.f, const float VolumeMultiplier = 1.f) const;
	void HandleJumpLaunch() const;
	void HandleLanding(const FName& BoneName, const EUpTraceDirection::Type TraceDirection = EUpTraceDirection::Down,
		const float TraceLength = 10.f, const float VolumeMultiplier = 1.f) const;

	void SetEffectHandle_BusyState(const FActiveGameplayEffectHandle& InHandle) { EffectHandle_BusyState = InHandle; }

	void AddRecoil(const FVector2D& Value, const float TimeToLive);
	void SetActiveSpecialMoveTag(const FGameplayTag& Tag);
	
	FORCEINLINE FGameplayTag GetActiveSpecialMoveTag() const { return ActiveSpecialMoveTag; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE FUpCharacterData GetCharacterData() const { return CharacterData; }
	FORCEINLINE UUpCharacterMovementComponent* GetCustomMovementComponent() const { return CustomMovementComponent; }
	FORCEINLINE FActiveGameplayEffectHandle GetEffectHandle_BusyState() const { return EffectHandle_BusyState; }
	FORCEINLINE FUpCharacterEquipment GetEquipment() const { return Equipment; }
	FORCEINLINE EUpCharacterPosture::Type GetPosture() const { return Posture; }
	FORCEINLINE FVector2D GetCurrentRecoil() const { return CurrentRecoil; }
	FORCEINLINE bool IsRelaxed() const { return bRelaxed; }
	FORCEINLINE bool ShouldDebugGas() const { return bDebugGas; }
	FORCEINLINE bool ShouldDebugMovement() const { return bDebugMovement; }
	
	FORCEINLINE FVector GetRootMotionTargetLocation() const { return RootMotionTargetLocation; }
	FORCEINLINE bool HasRootMotionTargetLocation() const { return bHasRootMotionTargetLocation; }
	
	FORCEINLINE UUpPrimaryAttributeSet* GetPrimaryAttributeSet() const { return PrimaryAttributeSet; }
	FORCEINLINE UUpVitalAttributeSet* GetVitalAttributeSet() const { return VitalAttributeSet; }
	
	FORCEINLINE UCapsuleComponent* GetHitCapsule_Body() const { return HitCapsule_Body; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpCombatComponent> CombatComponent_0;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> HitCapsule_Head;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> HitCapsule_Body;	
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> ClimbingMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> DeathsMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> GesturesMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> GunFiringMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> GunMeleeAttacksMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> HitReactionsMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> MantlesMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> SpecialMovesMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> ReloadsMontage_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> WeaponEquipMontage_ThirdPerson;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|GAS")
	TArray<TSubclassOf<UGameplayAbility>> AbilityClassesToGrant;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|GAS")
	TSubclassOf<UGameplayEffect> EffectClass_InitPrimaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|GAS")
	TSubclassOf<UGameplayEffect> EffectClass_InitVitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|SFX")
	TObjectPtr<USoundCue> Sfx_JumpLaunches;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|SFX")
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap_Footsteps;
	UPROPERTY(EditAnywhere, Category="UP Assets|SFX")
	TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap_JumpLandings;
	
	UPROPERTY(EditAnywhere, Category="UP Data")
	FUpCharacterEquipment Equipment;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugGas = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugMovement = false;
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugPhysicalMaterials = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag ActiveSpecialMoveTag = TAG_Ability_SpecialMove_Push;
	
	UPROPERTY(Transient)
	TObjectPtr<UUpCharacterMovementComponent> CustomMovementComponent;
	UPROPERTY(Transient)
	TEnumAsByte<EUpCharacterPosture::Type> Posture = EUpCharacterPosture::Casual;

	UPROPERTY(Transient)
	TArray<FName> ActiveHitBoxNames;
	UPROPERTY(Transient)
	TArray<AActor*> HitActors;

	UPROPERTY()
	TObjectPtr<UUpPrimaryAttributeSet> PrimaryAttributeSet;
	UPROPERTY()
	TObjectPtr<UUpVitalAttributeSet> VitalAttributeSet;
	
	FUpCharacterData CharacterData;
	FActiveGameplayEffectHandle EffectHandle_BusyState;
	bool bRelaxed = true;
	
	FVector RootMotionTargetLocation;
	bool bHasRootMotionTargetLocation = false;
	
	FVector2D CurrentRecoil = FVector2D(0.f, 0.f);
	FTimerHandle RecoilCenterTimerHandle;

	UFUNCTION()
	void HandleAbilitySystemTagEvent(const FGameplayTag Tag, int32 Count);

	virtual void GetAbilityClassesToGrant(TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses) const;

	AUpItem* SpawnAndAttachItem(const TSubclassOf<AUpItem> ItemClass);

	void HandleNoise(const TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap, const FName& BoneName,
		const EUpTraceDirection::Type TraceDirection = EUpTraceDirection::Down, const float TraceLength = 10.f, const float VolumeMultiplier = 1.f) const;
	
	virtual void OnItemEquip(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot);
	virtual void OnItemUnequip(const EUpEquipmentSlot::Type EquipmentSlot) {}
	virtual void OnEquipmentActivation(const EUpEquipmentSlot::Type EquipmentSlot) {}
	virtual void OnEquipmentDeactivation(const EUpEquipmentSlot::Type EquipmentSlot) {}
};
