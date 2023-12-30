// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpCharacter.h"
#include "Player/UpPlayerController.h"
#include "UpPlayableCharacter.generated.h"

class AUpWeapon;
class UCameraComponent;
class USpringArmComponent;
class UUpPlayerCombatComponent;
class UUpPlayerInteractionComponent;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayableCharacter : public AUpCharacter
{
	GENERATED_BODY()

public:
	explicit AUpPlayableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	UFUNCTION(BlueprintCallable)
	void ActivateCameraView(const EUpCameraView::Type CameraViewType);

	virtual void Jump() override;
	virtual void StopJumping() override;
	void AllowJump() { bAllowedToJump = true; }
	bool IsAllowedToJump() const { return bAllowedToJump; };

	virtual EUpCameraView::Type GetCameraView() const override;

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

	UPROPERTY(Transient)
	TObjectPtr<AUpWeapon> ActiveWeapon;
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomPlayerController;
	
	bool bAllowedToJump = true;
	bool bIsPlayer = false;

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
