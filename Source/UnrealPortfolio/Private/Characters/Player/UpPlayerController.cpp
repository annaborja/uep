// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/UpPlayerCameraManager.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Characters/Player/Components/UpPlayerMovementComponent.h"
#include "Interfaces/UpInteractable.h"
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

	check(CrouchInputAction);
	check(InteractInputAction);
	check(JumpInputAction);
	check(LookInputAction);
	check(MoveInputAction);
	check(PauseGameInputAction);
	check(SprintInputAction);

	CustomPlayer = CastChecked<AUpPlayerCharacter>(GetCharacter());

	CustomHud = CastChecked<AUpHud>(GetHUD());
	CustomHud->Init(this);
}

void AUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(PauseGameInputAction, ETriggerEvent::Completed, this, &ThisClass::PauseGame);
	
	EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &ThisClass::ToggleCrouch);
	EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Started, this, &ThisClass::Interact);
	EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::Jump);

	EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ThisClass::StartSprint);
	EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
	
	EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AUpPlayerController::PauseGame(const FInputActionValue& InputActionValue)
{
	if (CustomHud) CustomHud->OpenMainMenu();
	
	UGameplayStatics::SetGamePaused(this, true);
}

void AUpPlayerController::ToggleCrouch(const FInputActionValue& InputActionValue)
{
	if (!CustomPlayer) return;

	if (CustomPlayer->bIsCrouched)
	{
		CustomPlayer->UnCrouch();
	} else
	{
		CustomPlayer->Crouch();
	}
}

void AUpPlayerController::Interact(const FInputActionValue& InputActionValue)
{
	if (!CustomPlayer) return;
	
	if (const auto InteractionComponent = CustomPlayer->GetInteractionComponent())
	{
		if (const auto TargetInteractable = Cast<IUpInteractable>(InteractionComponent->GetTargetInteractable()))
		{
			TargetInteractable->Interact(this);
		}
	}
}

void AUpPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (!CustomPlayer) return;
	
	CustomPlayer->Jump();
}

void AUpPlayerController::StartSprint(const FInputActionValue& InputActionValue)
{
	if (!CustomPlayer) return;
	
	if (const auto PlayerMovementComponent = CustomPlayer->GetPlayerMovementComponent())
	{
		PlayerMovementComponent->ToggleSprint(true);
	}
}

void AUpPlayerController::StopSprint(const FInputActionValue& InputActionValue)
{
	if (!CustomPlayer) return;
	
	if (const auto PlayerMovementComponent = CustomPlayer->GetPlayerMovementComponent())
	{
		PlayerMovementComponent->ToggleSprint(false);
	}
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

void AUpPlayerController::ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting, const int32 Priority) const
{
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bClearExisting) Subsystem->ClearAllMappings();
		
		Subsystem->AddMappingContext(InputMappingContext, Priority);
	}
}
