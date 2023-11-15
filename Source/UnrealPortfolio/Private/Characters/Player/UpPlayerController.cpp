// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/UpPlayerCameraManager.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpHud.h"

AUpPlayerController::AUpPlayerController(): APlayerController()
{
	PlayerCameraManagerClass = AUpPlayerCameraManager::StaticClass();
}

void AUpPlayerController::Init() const
{
	ActivateInputMappingContext(BaseInputMappingContext);
}

void AUpPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(BaseInputMappingContext);

	check(LookInputAction);
	check(MoveInputAction);
	check(PauseGameInputAction);

	CustomPlayer = CastChecked<AUpPlayerCharacter>(GetCharacter());

	CustomHud = CastChecked<AUpHud>(GetHUD());
	CustomHud->Init(this);
}

void AUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(PauseGameInputAction, ETriggerEvent::Completed, this, &ThisClass::PauseGame);
	
	EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AUpPlayerController::PauseGame(const FInputActionValue& InputActionValue)
{
	if (CustomHud) CustomHud->OpenMainMenu();
	
	UGameplayStatics::SetGamePaused(this, true);
}

void AUpPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const auto InputActionVector = InputActionValue.Get<FVector2D>();

	AddPitchInput(InputActionVector.Y);
	AddYawInput(InputActionVector.X);
}

void AUpPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const auto InputActionVector = InputActionValue.Get<FVector2D>();
	const FRotationMatrix RotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f));

	if (CustomPlayer)
	{
		CustomPlayer->AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::X), InputActionVector.Y);
		CustomPlayer->AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::Y), InputActionVector.X);
	}
}

void AUpPlayerController::ActivateInputMappingContext(const UInputMappingContext* InputMappingContext,
	const bool bClearExisting, const int32 Priority) const
{
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bClearExisting) Subsystem->ClearAllMappings();
		
		Subsystem->AddMappingContext(InputMappingContext, Priority);
	}
}
