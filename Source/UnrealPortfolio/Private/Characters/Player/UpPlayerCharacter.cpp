// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
}

void AUpPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CustomController = CastChecked<AUpPlayerController>(NewController);
	CustomController->Init();
}
