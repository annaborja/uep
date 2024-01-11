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
	explicit AUpPlayableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void Jump() override;
	virtual void StopJumping() override;
	void AllowJump() { bAllowedToJump = true; }
	bool IsAllowedToJump() const { return bAllowedToJump; };

	virtual EUpCameraView::Type GetCameraView() const override;
	virtual UAnimMontage* GetMantlesMontage() const override;
	virtual UAnimMontage* GetReloadsMontage() const override;

	virtual bool GrantTagSpec(const FUpTagSpec& TagSpec) override;
	
	UFUNCTION(BlueprintCallable)
	void ActivateCameraView(const EUpCameraView::Type CameraViewType);

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return CameraSpringArm; }
	FORCEINLINE AUpPlayerController* GetCustomPlayerController() const { return CustomPlayerController; }
	FORCEINLINE FUpCharacterEquipment GetEquipment() const { return Equipment; }
	FORCEINLINE UUpPlayerInteractionComponent* GetPlayerInteractionComponent() const { return PlayerInteractionComponent; }
	FORCEINLINE UUpPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }
	FORCEINLINE bool IsPlayer() const { return bIsPlayer; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraSpringArm;

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
	TObjectPtr<UAnimMontage> MantlesMontage_FirstPerson;
	UPROPERTY(EditAnywhere, Category="UP Assets|Animation")
	TObjectPtr<UAnimMontage> ReloadsMontage_FirstPerson;
	
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebugTagSpecGrant = false;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomPlayerController;
	UPROPERTY(Transient)
	TObjectPtr<AUpWeapon> ActiveWeapon;
	
	bool bAllowedToJump = true;
	bool bIsPlayer = false;

	virtual void OnItemEquip(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot) override;
	virtual void OnItemUnequip(const EUpEquipmentSlot::Type EquipmentSlot) override;

	virtual void OnEquipmentActivation(const EUpEquipmentSlot::Type EquipmentSlot) override;
	virtual void OnEquipmentDeactivation(const EUpEquipmentSlot::Type EquipmentSlot) override;

private:
	TMap<FGameplayTag, FDelegateHandle> AttributeValueDelegateHandleMap;
	
	void InitForPlayer();
	void TearDownForPlayer();
	
	void SetUpFirstPersonMesh();
	void SetUpThirdPersonCamera() const;
	void SetUpThirdPersonMesh();

	void HandleWeaponDelegates(AUpWeapon* Weapon);
};
