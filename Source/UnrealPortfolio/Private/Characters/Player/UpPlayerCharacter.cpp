// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Characters/Player/Components/UpPlayerMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/GeneralStructs.h"

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

	PlayerMovementComponent = CastChecked<UUpPlayerMovementComponent>(GetCharacterMovement());

	InteractionComponent = CreateDefaultSubobject<UUpPlayerInteractionComponent>(TEXT("InteractionComponent"));
}

void AUpPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CustomController = CastChecked<AUpPlayerController>(NewController);
	CustomController->Init();
}

void AUpPlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->GetPlayerCharacterTags(TagContainer);
	}
}

void AUpPlayerCharacter::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (GameMode->ShouldDebugTagSpecGrant())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player GrantTagSpec: %s (%d)"), *TagSpec.Tag.ToString(), TagSpec.Count)
		}
	
		if (TagSpec.Tag.IsValid())
		{
			bool bSuccess = false;

			if (TagSpec.Count > 0)
			{
				bSuccess = GameMode->AddPlayerCharacterTag(TagSpec.Tag);
			} else if (TagSpec.Count < 0)
			{
				bSuccess = GameMode->RemovePlayerCharacterTag(TagSpec.Tag);
			}
		}
	}
}

AUpHud* AUpPlayerCharacter::GetCustomHud() const
{
	if (CustomController) return CustomController->GetCustomHud();

	return nullptr;
}
