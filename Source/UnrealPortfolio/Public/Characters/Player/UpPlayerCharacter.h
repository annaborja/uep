// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCharacter : public AUpCharacter, public IGameplayTagAssetInterface, public IUpTagSpecGrantable
{
	GENERATED_BODY()

public:
	explicit AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual void GrantTagSpec(const FUpTagSpec& TagSpec) override;

	AUpHud* GetCustomHud() const;

	FORCEINLINE AUpPlayerController* GetCustomController() const { return CustomController; }
	FORCEINLINE UUpPlayerInteractionComponent* GetPlayerInteractionComponent() const { return InteractionComponent; }
	FORCEINLINE UUpPlayerMovementComponent* GetPlayerMovementComponent() const { return PlayerMovementComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> FollowCameraSpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUpPlayerInteractionComponent> InteractionComponent;

	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> CustomController;
	UPROPERTY(Transient)
	TObjectPtr<UUpPlayerMovementComponent> PlayerMovementComponent;
};
