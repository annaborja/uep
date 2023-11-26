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

	AUpHud* GetCustomHud() const;

	void AllowJump();
	bool IsAllowedToJump() const { return bAllowedToJump; };
	
	FORCEINLINE AUpPlayerController* GetCustomController() const { return CustomController; }
	FORCEINLINE UUpPlayerInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	FORCEINLINE UUpPlayerCombatComponent* GetPlayerCombatComponent() const { return CombatComponent; }
	FORCEINLINE UUpPlayerMovementComponent* GetPlayerMovementComponent() const { return PlayerMovementComponent; }
	FORCEINLINE UUpPlayerReputationComponent* GetReputationComponent() const { return ReputationComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> FollowCameraSpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerInteractionComponent> InteractionComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerReputationComponent> ReputationComponent;

	UPROPERTY(VisibleInstanceOnly, Category="UP Runtime")
	bool bAllowedToJump = true;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
	UPROPERTY(Transient)
	TObjectPtr<UUpPlayerMovementComponent> PlayerMovementComponent;
};
