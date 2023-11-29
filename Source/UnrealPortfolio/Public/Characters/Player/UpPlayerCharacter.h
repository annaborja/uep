// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/UpCharacter.h"
#include "Characters/Player/Components/UpPlayerCombatComponent.h"
#include "Interfaces/UpCombatable.h"
#include "Interfaces/UpTagSpecGrantable.h"
#include "UpPlayerCharacter.generated.h"

class AUpHud;
class AUpPlayerController;
class UCameraComponent;
class USpringArmComponent;
class UUpPlayerInteractionComponent;
class UUpPlayerMovementComponent;
class UUpPlayerReputationComponent;

UENUM(BlueprintType)
namespace EUpPlayerCameraViewType
{
	enum Type : uint8
	{
		FirstPerson,
		ThirdPerson,
		ThirdPersonShoulder
	};
}

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface,
	public IUpCombatable, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Jump() override;
	virtual void StopJumping() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UUpCombatComponent* GetCombatComponent() const override { return CombatComponent; }

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	UFUNCTION(BlueprintCallable)
	EUpPlayerCameraViewType::Type ActivateCameraView(const EUpPlayerCameraViewType::Type CameraViewType);

	AUpHud* GetCustomHud() const;
	void ToggleCameraView();

	void AllowJump() { bAllowedToJump = true; }
	bool IsAllowedToJump() const { return bAllowedToJump; };

	FORCEINLINE EUpPlayerCameraViewType::Type GetCurrentCameraViewType() const { return CurrentCameraViewType; }
	FORCEINLINE AUpPlayerController* GetCustomController() const { return CustomController; }
	FORCEINLINE UUpPlayerInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	FORCEINLINE UUpPlayerCombatComponent* GetPlayerCombatComponent() const { return CombatComponent; }
	FORCEINLINE UUpPlayerMovementComponent* GetPlayerMovementComponent() const { return PlayerMovementComponent; }
	FORCEINLINE UUpPlayerReputationComponent* GetReputationComponent() const { return ReputationComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraSpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerInteractionComponent> InteractionComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerReputationComponent> ReputationComponent;

	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UAnimInstance> AnimClass_FirstPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UAnimInstance> AnimClass_ThirdPerson;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USkeletalMesh> SkeletalMesh_FirstPerson;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<USkeletalMesh> SkeletalMesh_ThirdPerson;
	
	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	bool bAllowedToJump = true;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
	UPROPERTY(Transient)
	TObjectPtr<UUpPlayerMovementComponent> PlayerMovementComponent;
	
	TEnumAsByte<EUpPlayerCameraViewType::Type> CurrentCameraViewType = EUpPlayerCameraViewType::ThirdPerson;
};
