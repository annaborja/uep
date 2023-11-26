// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCharacter.h"

#include "UpGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
#include "Characters/Player/Components/UpPlayerCombatComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Characters/Player/Components/UpPlayerMovementComponent.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utils/Structs.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpPlayerCharacter::AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpPlayerMovementComponent>(CharacterMovementComponentName))
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	if (const auto Mesh = GetMesh())
	{
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
		Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
	
	FollowCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowCameraSpringArm"));
	FollowCameraSpringArm->SetupAttachment(GetRootComponent());
	FollowCameraSpringArm->TargetArmLength = 300.f;
	FollowCameraSpringArm->SocketOffset = FVector(0.f, 0.f, 108.f);
	FollowCameraSpringArm->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(FollowCameraSpringArm);
	FollowCamera->SetRelativeRotation(FRotator(-8.f, 0.f, 0.f));

	CombatComponent = CreateDefaultSubobject<UUpPlayerCombatComponent>(TEXT("CombatComponent"));
	InteractionComponent = CreateDefaultSubobject<UUpPlayerInteractionComponent>(TEXT("InteractionComponent"));
	ReputationComponent = CreateDefaultSubobject<UUpPlayerReputationComponent>(TEXT("ReputationComponent"));
}

void AUpPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(InitVitalAttributesEffectClass);
	check(InitPrimaryAttributesEffectClass);
	
	PlayerMovementComponent = CastChecked<UUpPlayerMovementComponent>(GetCharacterMovement());
}

void AUpPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (const auto CustomPlayerState = GetPlayerState<AUpPlayerState>())
	{
		CastChecked<UUpAbilitySystemComponent>(CustomPlayerState->GetAbilitySystemComponent())->Init(CustomPlayerState, this,
			TArray { InitVitalAttributesEffectClass, InitPrimaryAttributesEffectClass },
			TArray<TSubclassOf<UGameplayAbility>> {});
	}

	CustomController = CastChecked<AUpPlayerController>(NewController);
	CustomController->Init();
}

void AUpPlayerCharacter::Jump()
{
	if (!bAllowedToJump) return;

	bAllowedToJump = false;
	
	if (PlayerMovementComponent)
	{
		PlayerMovementComponent->bNotifyApex = true;
		PlayerMovementComponent->ToggleCustomPressedJump(true);
	}
	
	Super::Jump();

	// Override the default jump logic with our custom logic.
	bPressedJump = false;
}

void AUpPlayerCharacter::StopJumping()
{
	if (PlayerMovementComponent) PlayerMovementComponent->ToggleCustomPressedJump(false);
	
	Super::StopJumping();
}

UAbilitySystemComponent* AUpPlayerCharacter::GetAbilitySystemComponent() const
{
	if (const auto CustomPlayerState = GetPlayerState<AUpPlayerState>())
	{
		return CustomPlayerState->GetAbilitySystemComponent();
	}

	return nullptr;
}

void AUpPlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->GetPlayerCharacterTags(TagContainer);
	}

	if (const auto AbilitySystemComponent = GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);
		TagContainer.AppendTags(AbilitySystemTags);
	}
}

void AUpPlayerCharacter::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (GameInstance->ShouldDebugTagSpecGrant())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player GrantTagSpec: %s (%d)"), *TagSpec.Tag.ToString(), TagSpec.Count)
		}
	
		bool bSuccess = false;

		if (UUpPlayerReputationComponent::ShouldHandleTagSpecGrant(TagSpec))
		{
			bSuccess = ReputationComponent->HandleTagSpecGrant(TagSpec);
		} else if (TagSpec.Count > 0)
		{
			bSuccess = GameInstance->AddPlayerCharacterTag(TagSpec.Tag);
		} else if (TagSpec.Count < 0)
		{
			bSuccess = GameInstance->RemovePlayerCharacterTag(TagSpec.Tag);
		}
	}
}

AUpHud* AUpPlayerCharacter::GetCustomHud() const
{
	if (CustomController) return CustomController->GetCustomHud();

	return nullptr;
}

void AUpPlayerCharacter::AllowJump()
{
	bAllowedToJump = true;
}
