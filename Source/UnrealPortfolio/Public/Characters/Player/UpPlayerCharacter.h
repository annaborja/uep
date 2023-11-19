// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Characters/UpCharacter.h"
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
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpCharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	AUpHud* GetCustomHud() const;

	FORCEINLINE AUpPlayerController* GetCustomController() const { return CustomController; }
	FORCEINLINE UUpPlayerInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	FORCEINLINE UUpPlayerMovementComponent* GetPlayerMovementComponent() const { return PlayerMovementComponent; }
	FORCEINLINE UUpPlayerReputationComponent* GetReputationComponent() const { return ReputationComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> FollowCameraSpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerInteractionComponent> InteractionComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerReputationComponent> ReputationComponent;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
	UPROPERTY(Transient)
	TObjectPtr<UUpPlayerMovementComponent> PlayerMovementComponent;
};
