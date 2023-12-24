// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCharacter.h"

#include "UpGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/Components/UpPlayerCombatComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpPlayerCharacter::AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);

	PlayerCombatComponent = CreateDefaultSubobject<UUpPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerInteractionComponent = CreateDefaultSubobject<UUpPlayerInteractionComponent>(TEXT("PlayerInteractionComponent"));
}

void AUpPlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->GetPlayerCharacterTags(TagContainer);
	}

	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);
		TagContainer.AppendTags(AbilitySystemTags);
	}
}

void AUpPlayerCharacter::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	UUpBlueprintFunctionLibrary::GrantPlayerTagSpec(this, TagSpec);
}
