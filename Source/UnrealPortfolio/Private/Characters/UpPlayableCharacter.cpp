// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

void AUpPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(AnimClass_FirstPerson);
	check(AnimClass_ThirdPerson);
	check(SkeletalMesh_FirstPerson);
	check(SkeletalMesh_ThirdPerson);
	
	check(InitVitalAttributesEffectClass);
	check(InitPrimaryAttributesEffectClass);
	
	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());
	
	if (bIsPlayer) CustomMovementComponent->InitForPlayer();
}

void AUpPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	CustomPlayerController = Cast<AUpPlayerController>(NewController);

	if (CustomPlayerController) InitForPlayer();
}

void AUpPlayableCharacter::UnPossessed()
{
	Super::UnPossessed();

	if (CustomPlayerController) TearDownForPlayer();
}

void AUpPlayableCharacter::ActivateCameraView(const EUpPlayerCameraViewType::Type CameraViewType)
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

		if (CameraSpringArm)
		{
			CameraSpringArm->TargetArmLength = 5.f;
			CameraSpringArm->SocketOffset = FVector(10.f, 0.f, 70.f);
			CameraSpringArm->bUsePawnControlRotation = true;
		}

		if (Camera)
		{
			Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
			Camera->bUsePawnControlRotation = false;
		}

		bUseControllerRotationPitch = true;
		bUseControllerRotationRoll = true;
		bUseControllerRotationYaw = true;
		break;
	case EUpPlayerCameraViewType::ThirdPerson:
		SetUpThirdPersonMesh();

		if (CameraSpringArm)
		{
			CameraSpringArm->TargetArmLength = 300.f;
			CameraSpringArm->SocketOffset = FVector(0.f, 0.f, 108.f);
			CameraSpringArm->bUsePawnControlRotation = true;
		}

		if (Camera)
		{
			Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			Camera->SetRelativeRotation(FRotator(-8.f, 0.f, 0.f));
			Camera->bUsePawnControlRotation = false;
		}

		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;
		break;
	case EUpPlayerCameraViewType::ThirdPerson_OverTheShoulder:
		SetUpThirdPersonMesh();

		if (CameraSpringArm)
		{
			CameraSpringArm->TargetArmLength = 100.f;
			CameraSpringArm->SocketOffset = FVector(0.f, 0.f, 108.f);
			CameraSpringArm->bUsePawnControlRotation = true;
		}

		if (Camera)
		{
			Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			Camera->SetRelativeRotation(FRotator(-8.f, 0.f, 0.f));
			Camera->bUsePawnControlRotation = false;
		}

		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid player camera view type %d"), CameraViewType)
	}

	if (CustomPlayerController) CustomPlayerController->SetCurrentCameraViewType(CameraViewType);
}

void AUpPlayableCharacter::Jump()
{
	if (bIsPlayer)
	{
		if (!bAllowedToJump) return;

		bAllowedToJump = false;
	
		if (CustomMovementComponent)
		{
			CustomMovementComponent->bNotifyApex = true;
			CustomMovementComponent->ToggleCustomPressedJump(true);
		}
	
		Super::Jump();

		// Override the default jump logic with our custom logic.
		bPressedJump = false;
	} else
	{
		Super::Jump();
	}
}

void AUpPlayableCharacter::StopJumping()
{
	if (bIsPlayer && CustomMovementComponent) CustomMovementComponent->ToggleCustomPressedJump(false);
	
	Super::StopJumping();
}

void AUpPlayableCharacter::InitForPlayer()
{
	bIsPlayer = true;
		
	if (CustomPlayerController && !CustomPlayerController->IsInitialized())
	{
		if (const auto CustomPlayerState = GetPlayerState<AUpPlayerState>())
		{
			TArray<TSubclassOf<UGameplayEffect>> InitAttributesEffectClasses;
			if (InitVitalAttributesEffectClass) InitAttributesEffectClasses.Add(InitVitalAttributesEffectClass);
			if (InitPrimaryAttributesEffectClass) InitAttributesEffectClasses.Add(InitPrimaryAttributesEffectClass);
			
			CastChecked<UUpAbilitySystemComponent>(CustomPlayerState->GetAbilitySystemComponent())->Init(
				CustomPlayerState, this, InitAttributesEffectClasses, TArray<TSubclassOf<UGameplayAbility>> {});
		}
		
		CustomPlayerController->Init();
	}

	if (!IsValid(CameraSpringArm))
	{
		CameraSpringArm = NewObject<USpringArmComponent>(this, USpringArmComponent::StaticClass(), NAME_None, RF_Transient);
		CameraSpringArm->SetupAttachment(GetRootComponent());
		CameraSpringArm->RegisterComponent();
	}

	if (!IsValid(Camera))
	{
		Camera = NewObject<UCameraComponent>(this, UCameraComponent::StaticClass(), NAME_None, RF_Transient);
		Camera->SetupAttachment(CameraSpringArm);
		Camera->RegisterComponent();
	}

	if (!IsValid(PlayerInteractionComponent))
	{
		PlayerInteractionComponent = NewObject<UUpPlayerInteractionComponent>(this, UUpPlayerInteractionComponent::StaticClass(), NAME_None, RF_Transient);
		PlayerInteractionComponent->RegisterComponent();
	}

	if (CustomPlayerController) ActivateCameraView(CustomPlayerController->GetCurrentCameraViewType());
		
	if (CustomMovementComponent) CustomMovementComponent->InitForPlayer();
}

void AUpPlayableCharacter::TearDownForPlayer()
{
	CustomPlayerController = nullptr;
	bIsPlayer = false;

	SetUpThirdPersonMesh();
	
	if (!Cast<AUpPlayerCharacter>(this))
	{
		if (IsValid(CameraSpringArm))
		{
			CameraSpringArm->UnregisterComponent();
			CameraSpringArm->DestroyComponent();
		}

		if (IsValid(Camera))
		{
			Camera->UnregisterComponent();
			Camera->DestroyComponent();
		}

		if (IsValid(PlayerInteractionComponent))
		{
			PlayerInteractionComponent->UnregisterComponent();
			PlayerInteractionComponent->DestroyComponent();
		}
	}
	
	if (CustomMovementComponent) CustomMovementComponent->TearDownForPlayer();
}

void AUpPlayableCharacter::SetUpThirdPersonMesh() const
{
	if (const auto Mesh = GetMesh())
	{
		Mesh->SetSkeletalMesh(SkeletalMesh_ThirdPerson);
		Mesh->SetAnimClass(AnimClass_ThirdPerson);
		Mesh->SetCastShadow(true);
	}
}
