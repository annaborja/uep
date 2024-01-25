// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpPlayableCharacter.generated.h"

class AUpPlayerController;
class AUpWeapon;
class UCameraComponent;
class USpringArmComponent;
class UUpPlayerCombatComponent;
class UUpPlayerInteractionComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayableCharacter : public AUpCharacter, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayableCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void Jump() override;
	virtual void StopJumping() override;
	void AllowJump() { bAllowedToJump = true; }
	bool IsAllowedToJump() const { return bAllowedToJump; };

	virtual EUpCameraView::Type GetCameraView() const override;
	virtual UAnimMontage* GetClimbingMontage() const override;
	virtual UAnimMontage* GetGunFiringMontage() const override;
	virtual UAnimMontage* GetMantlesMontage() const override;
	virtual UAnimMontage* GetReloadsMontage() const override;
	virtual UAnimMontage* GetWeaponEquipMontage() const override;

	virtual bool GrantTagSpec(const FUpTagSpec& TagSpec) override;
	
	UFUNCTION(BlueprintCallable)
	void ActivateCameraView(const EUpCameraView::Type CameraViewType);

	bool CanShoot() const;

	FORCEINLINE float GetBaseCameraFov() const { return BaseCameraFov; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return SpringArm; }
	FORCEINLINE AUpPlayerController* GetCustomPlayerController() const { return CustomPlayerController; }
	FORCEINLINE UUpPlayerInteractionComponent* GetPlayerInteractionComponent() const { return PlayerInteractionComponent; }
	FORCEINLINE UUpPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }
	FORCEINLINE bool IsPlayer() const { return bIsPlayer; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerCombatComponent> PlayerCombatComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerInteractionComponent> PlayerInteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UAnimInstance> AnimClass_FirstPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UAnimInstance> AnimClass_ThirdPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USkeletalMesh> SkeletalMesh_FirstPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USkeletalMesh> SkeletalMesh_ThirdPerson;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> ClimbingMontage_FirstPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> GunFiringMontage_FirstPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> MantlesMontage_FirstPerson;
	UPROPERTY(EditAnywhere, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> ReloadsMontage_FirstPerson;
	UPROPERTY(EditAnywhere, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> WeaponEquipMontage_FirstPerson;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugTagSpecGrant = false;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float BaseCameraFov = 90.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float CameraBlendDuration = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float SpringArmLength_FirstPerson = 5.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	FVector SpringArmSocketOffset_FirstPerson = FVector(10.f, 0.f, 80.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FVector CameraRelativeLocation_FirstPerson = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FRotator CameraRelativeRotation_FirstPerson = FRotator(0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float SpringArmLength_ThirdPerson = 100.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	FVector SpringArmSocketOffset_ThirdPerson = FVector(0.f, 55.f, 70.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FVector CameraRelativeLocation_ThirdPerson = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FRotator CameraRelativeRotation_ThirdPerson = FRotator(0.f, 0.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float SpringArmLength_Debug = 300.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	FVector SpringArmSocketOffset_Debug = FVector(0.f, 0.f, 108.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FVector CameraRelativeLocation_Debug = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FRotator CameraRelativeRotation_Debug = FRotator(-8.f, 0.f, 0.f);

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomPlayerController;
	UPROPERTY(Transient)
	TObjectPtr<AUpWeapon> ActiveWeapon;
	
	bool bAllowedToJump = true;
	bool bIsPlayer = false;

	virtual void GetAbilityClassesToGrant(TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses) const override;

	virtual void OnItemEquip(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot) override;
	virtual void OnItemUnequip(const EUpEquipmentSlot::Type EquipmentSlot) override;

	virtual void OnEquipmentActivation(const EUpEquipmentSlot::Type EquipmentSlot) override;
	virtual void OnEquipmentDeactivation(const EUpEquipmentSlot::Type EquipmentSlot) override;

private:
	TMap<FGameplayTag, FDelegateHandle> AttributeValueDelegateHandleMap;
	TScriptDelegate<FWeakObjectPtr> CapsuleHitDelegate;

	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	float CurrentCameraBlendTime = -1.f;
	
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	float PrevSpringArmLength = -1.f;
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	float TargetSpringArmLength = -1.f;
	
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	FVector PrevSpringArmSocketOffset = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	FVector TargetSpringArmSocketOffset = FVector::ZeroVector;
	
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	FVector PrevCameraRelativeLocation = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	FVector TargetCameraRelativeLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	FRotator PrevCameraRelativeRotation = FRotator::ZeroRotator;
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	FRotator TargetCameraRelativeRotation = FRotator::ZeroRotator;

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void InitForPlayer();
	void TearDownForPlayer();

	void SetCameraParams(const float SpringArmLength, const FVector& SpringArmSocketOffset, const FVector& CameraRelativeLocation, const FRotator& CameraRelativeRotation);
	void SetUpCharacterForCameraView();
	void SetUpFirstPersonController();
	void SetUpFirstPersonMesh();
	void SetUpThirdPersonController();
	void SetUpThirdPersonMesh();
	void SetUpThirdPersonCamera();

	void HandleWeaponDelegates(AUpWeapon* Weapon);
};
