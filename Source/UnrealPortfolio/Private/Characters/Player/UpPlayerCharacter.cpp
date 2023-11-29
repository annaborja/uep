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

	if (const auto Mesh = GetMesh())
	{
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
		Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
	
	CombatComponent = CreateDefaultSubobject<UUpPlayerCombatComponent>(TEXT("CombatComponent"));
	InteractionComponent = CreateDefaultSubobject<UUpPlayerInteractionComponent>(TEXT("InteractionComponent"));
	ReputationComponent = CreateDefaultSubobject<UUpPlayerReputationComponent>(TEXT("ReputationComponent"));
	
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);
}

void AUpPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(AnimClass_FirstPerson);
	check(AnimClass_ThirdPerson);
	check(SkeletalMesh_FirstPerson);
	check(SkeletalMesh_ThirdPerson);
	
	check(InitVitalAttributesEffectClass);
	check(InitPrimaryAttributesEffectClass);
	
	PlayerMovementComponent = CastChecked<UUpPlayerMovementComponent>(GetCharacterMovement());
	
	ActivateCameraView(EUpPlayerCameraViewType::ThirdPerson);
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

EUpPlayerCameraViewType::Type AUpPlayerCharacter::ActivateCameraView(const EUpPlayerCameraViewType::Type CameraViewType)
{
	switch (CameraViewType)
	{
	case EUpPlayerCameraViewType::FirstPerson:
		if (const auto Mesh = GetMesh())
		{
			Mesh->SetSkeletalMesh(SkeletalMesh_FirstPerson);
			Mesh->SetAnimClass(AnimClass_FirstPerson);
			Mesh->SetCastShadow(false);
		}
		
		CameraSpringArm->TargetArmLength = 5.f;
		CameraSpringArm->SocketOffset = FVector(10.f, 0.f, 70.f);
		CameraSpringArm->bUsePawnControlRotation = true;

		Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		Camera->bUsePawnControlRotation = false;

		bUseControllerRotationYaw = true;
		break;
	case EUpPlayerCameraViewType::ThirdPerson:
		if (const auto Mesh = GetMesh())
		{
			Mesh->SetSkeletalMesh(SkeletalMesh_ThirdPerson);
			Mesh->SetAnimClass(AnimClass_ThirdPerson);
			Mesh->SetCastShadow(true);
		}
		
		CameraSpringArm->TargetArmLength = 300.f;
		CameraSpringArm->SocketOffset = FVector(0.f, 0.f, 108.f);
		CameraSpringArm->bUsePawnControlRotation = true;
	
		Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Camera->SetRelativeRotation(FRotator(-8.f, 0.f, 0.f));
		Camera->bUsePawnControlRotation = false;

		bUseControllerRotationYaw = false;
		break;
	case EUpPlayerCameraViewType::ThirdPersonShoulder:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid player camera view type %d"), CameraViewType)
	}

	return CameraViewType;
}

AUpHud* AUpPlayerCharacter::GetCustomHud() const
{
	if (CustomController) return CustomController->GetCustomHud();

	return nullptr;
}

void AUpPlayerCharacter::ToggleCameraView()
{
	switch (CurrentCameraViewType)
	{
	case EUpPlayerCameraViewType::FirstPerson:
		CurrentCameraViewType = ActivateCameraView(EUpPlayerCameraViewType::ThirdPerson);
		break;
	case EUpPlayerCameraViewType::ThirdPerson:
		CurrentCameraViewType = ActivateCameraView(EUpPlayerCameraViewType::ThirdPersonShoulder);
		break;
	case EUpPlayerCameraViewType::ThirdPersonShoulder:
		CurrentCameraViewType = ActivateCameraView(EUpPlayerCameraViewType::FirstPerson);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid player camera view type %d"), CurrentCameraViewType)
	}
}
