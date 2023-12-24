// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/SpringArmComponent.h"

void AUpPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(AnimClass_FirstPerson);
	check(AnimClass_ThirdPerson);
	check(SkeletalMesh_FirstPerson);
	check(SkeletalMesh_ThirdPerson);
	
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

void AUpPlayableCharacter::ActivateCameraView(const EUpCameraView::Type InCameraView)
{
	switch (InCameraView)
	{
	case EUpCameraView::FirstPerson:
		SetUpFirstPersonMesh();

		if (CameraSpringArm)
		{
			CameraSpringArm->TargetArmLength = 5.f;
			CameraSpringArm->SocketOffset = FVector(10.f, 0.f, 75.f);
			CameraSpringArm->bUsePawnControlRotation = true;
		}

		if (Camera)
		{
			Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
			Camera->bUsePawnControlRotation = false;
		}

		break;
	case EUpCameraView::ThirdPerson:
		SetUpThirdPersonMesh();
		
		if (CustomMovementComponent)
		{
			CustomMovementComponent->bOrientRotationToMovement = true;
		}
		
		SetUpThirdPersonCamera();
		
		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder:
		SetUpThirdPersonMesh();

		if (CustomMovementComponent)
		{
			CustomMovementComponent->bOrientRotationToMovement = false;
		}
	
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = true;

		if (CameraSpringArm)
		{
			CameraSpringArm->TargetArmLength = 100.f;
			CameraSpringArm->SocketOffset = FVector(0.f, 55.f, 70.f);
			CameraSpringArm->bUsePawnControlRotation = true;
		}

		if (Camera)
		{
			Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
			Camera->bUsePawnControlRotation = false;
		}

		break;
	case EUpCameraView::FirstPerson_Debug:
		SetUpFirstPersonMesh();
		SetUpThirdPersonCamera();
		
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid camera view %d"), InCameraView)
	}

	if (CustomPlayerController) CustomPlayerController->SetCameraView(InCameraView);
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

EUpCameraView::Type AUpPlayableCharacter::GetCameraView() const
{
	if (CustomPlayerController) return CustomPlayerController->GetCameraView();
	
	return Super::GetCameraView();
}

void AUpPlayableCharacter::InitForPlayer()
{
	bIsPlayer = true;
		
	if (CustomPlayerController && !CustomPlayerController->IsInitialized()) CustomPlayerController->Init();

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

	if (CustomMovementComponent) CustomMovementComponent->InitForPlayer();
	
	if (CustomPlayerController) ActivateCameraView(CustomPlayerController->GetCameraView());
}

void AUpPlayableCharacter::TearDownForPlayer()
{
	if (CustomPlayerController && !CustomPlayerController->IsDebugCameraActive()) SetUpThirdPersonMesh();
	
	CustomPlayerController = nullptr;
	bIsPlayer = false;
	
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

void AUpPlayableCharacter::SetUpFirstPersonMesh()
{
	if (const auto Mesh = GetMesh())
	{
		Mesh->SetSkeletalMesh(SkeletalMesh_FirstPerson);
		Mesh->SetAnimClass(AnimClass_FirstPerson);
		Mesh->SetCastShadow(false);
	}

	if (WeaponActor) WeaponActor->GetStaticMeshComponent()->SetCastShadow(false);
	
	if (CustomMovementComponent)
	{
		CustomMovementComponent->bOrientRotationToMovement = false;
	}

	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;
}

void AUpPlayableCharacter::SetUpThirdPersonCamera() const
{
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
}

void AUpPlayableCharacter::SetUpThirdPersonMesh()
{
	// In first-person mode, the character actor follows the controller rotation.
	// Make sure we don't retain any pitch rotation when switching from first-person to third-person.
	const auto CurrentActorRotation = GetActorRotation();
	SetActorRotation(FRotator(0.f, CurrentActorRotation.Yaw, CurrentActorRotation.Roll));
	
	if (const auto Mesh = GetMesh())
	{
		Mesh->SetSkeletalMesh(SkeletalMesh_ThirdPerson);
		Mesh->SetAnimClass(AnimClass_ThirdPerson);
		Mesh->SetCastShadow(true);
	}
	
	if (WeaponActor) WeaponActor->GetStaticMeshComponent()->SetCastShadow(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
