// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerCameraManager.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "Interfaces/UpInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "Tags/CombatTags.h"
#include "UI/UpHud.h"

AUpPlayerController::AUpPlayerController(): APlayerController()
{
	PlayerCameraManagerClass = AUpPlayerCameraManager::StaticClass();
}

void AUpPlayerController::Init()
{
	if (BaseInputMappingContext) ActivateInputMappingContext(BaseInputMappingContext);
	
	bInitialized = true;
}

void AUpPlayerController::EnableMouse()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AUpPlayerController::DisableMouse()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

bool AUpPlayerController::IsDebugCameraActive() const
{
	return IsValid(DebugPawn);
}

void AUpPlayerController::CloseCharacterSwitcher()
{
	if (!CustomHud) return;

	CustomHud->CloseCharacterSwitcher();
	DisableMouse();

	if (BaseInputMappingContext) ActivateInputMappingContext(BaseInputMappingContext);
}

void AUpPlayerController::SwitchCharacter(AUpNpcCharacter* Npc)
{
	CloseCharacterSwitcher();
	Possess(Npc);
}

void AUpPlayerController::ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting, const int32 Priority) const
{
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bClearExisting) Subsystem->ClearAllMappings();
		
		Subsystem->AddMappingContext(InputMappingContext, Priority);
	}
}

void AUpPlayerController::DeactivateInputMappingContext(const UInputMappingContext* InputMappingContext) const
{
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(InputMappingContext);
	}
}

void AUpPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(BaseInputMappingContext);
	check(CharacterSwitcherInputMappingContext);
	check(GunInputMappingContext);

	check(AimGunInputAction);
	check(CloseCharacterSwitcherInputAction);
	check(CrouchInputAction);
	check(FireGunInputAction);
	check(InteractInputAction);
	check(JumpInputAction);
	check(LookInputAction);
	check(MoveInputAction);
	check(NavigateCharacterSwitcherInputAction);
	check(OpenCharacterSwitcherInputAction);
	check(PauseGameInputAction);
	check(SprintInputAction);
	check(ToggleCameraViewInputAction);
	check(ToggleDebugCameraInputAction);
	check(Weapon1InputAction);
	check(Weapon2InputAction);

	CustomHud = CastChecked<AUpHud>(GetHUD());
	CustomHud->Init(this);
}

void AUpPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	PossessedCharacter = Cast<AUpPlayableCharacter>(InPawn);
}

void AUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(ToggleCameraViewInputAction, ETriggerEvent::Started, this, &ThisClass::ToggleCameraView);
	EnhancedInputComponent->BindAction(ToggleDebugCameraInputAction, ETriggerEvent::Started, this, &ThisClass::ToggleDebugCamera);
	
	EnhancedInputComponent->BindAction(PauseGameInputAction, ETriggerEvent::Completed, this, &ThisClass::PauseGame);
	EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ThisClass::Interact);
	
	EnhancedInputComponent->BindAction(OpenCharacterSwitcherInputAction, ETriggerEvent::Triggered, this, &ThisClass::OpenCharacterSwitcher);
	EnhancedInputComponent->BindAction(CloseCharacterSwitcherInputAction, ETriggerEvent::Triggered, this, &ThisClass::TriggerCloseCharacterSwitcher);
	EnhancedInputComponent->BindAction(NavigateCharacterSwitcherInputAction, ETriggerEvent::Triggered, this, &ThisClass::NavigateCharacterSwitcher);
	
	EnhancedInputComponent->BindAction(Weapon1InputAction, ETriggerEvent::Started, this, &ThisClass::ToggleWeapon1);
	EnhancedInputComponent->BindAction(Weapon2InputAction, ETriggerEvent::Started, this, &ThisClass::ToggleWeapon2);
	
	EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &ThisClass::ToggleCrouch);
	EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::Jump);

	EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ThisClass::StartSprint);
	EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
	
	EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	
	EnhancedInputComponent->BindAction(AimGunInputAction, ETriggerEvent::Started, this, &ThisClass::StartAimingGun);
	EnhancedInputComponent->BindAction(AimGunInputAction, ETriggerEvent::Completed, this, &ThisClass::StopAimingGun);
	
	EnhancedInputComponent->BindAction(FireGunInputAction, ETriggerEvent::Started, this, &ThisClass::StartFiringGun);
	EnhancedInputComponent->BindAction(FireGunInputAction, ETriggerEvent::Completed, this, &ThisClass::StopFiringGun);
}

void AUpPlayerController::ToggleCameraView(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	switch (CameraView)
	{
	case EUpCameraView::FirstPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson);
		break;
	case EUpCameraView::ThirdPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson_OverTheShoulder);
		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder:
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid player camera view type %d"), CameraView)
	}
}

void AUpPlayerController::ToggleDebugCamera(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (CameraView == EUpCameraView::FirstPerson_Debug)
	{
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson);
	} else
	{
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson_Debug);
	}
	
	// if (IsValid(DebugPawn))
	// {
	// 	if (DebugCharacter)
	// 	{
	// 		Possess(DebugCharacter);
	// 		DebugCharacter = nullptr;
	// 		DebugPawn->Destroy();
	// 	}
	// } else if (const auto World = GetWorld(); World && PossessedCharacter)
	// {
	// 	DebugCharacter = PossessedCharacter;
	// 	DebugPawn = World->SpawnActor<ADefaultPawn>(PossessedCharacter->GetActorLocation(), PossessedCharacter->GetActorRotation());
	// 	Possess(DebugPawn);
	// }
}

void AUpPlayerController::PauseGame(const FInputActionValue& InputActionValue)
{
	if (CustomHud) CustomHud->OpenMainMenu();
	
	UGameplayStatics::SetGamePaused(this, true);
}

void AUpPlayerController::Interact(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	if (const auto InteractionComponent = PossessedCharacter->GetPlayerInteractionComponent())
	{
		if (const auto TargetInteractable = Cast<IUpInteractable>(InteractionComponent->GetTargetInteractable()))
		{
			TargetInteractable->Interact(this);
		}
	}
}

void AUpPlayerController::OpenCharacterSwitcher(const FInputActionValue& InputActionValue)
{
	if (!CustomHud) return;

	CustomHud->OpenCharacterSwitcher();
	EnableMouse();

	if (CharacterSwitcherInputMappingContext) ActivateInputMappingContext(CharacterSwitcherInputMappingContext);
}

void AUpPlayerController::TriggerCloseCharacterSwitcher(const FInputActionValue& InputActionValue)
{
	CloseCharacterSwitcher();
}

void AUpPlayerController::NavigateCharacterSwitcher(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("[temp] navigate char switcher"))
}

void AUpPlayerController::ToggleWeapon1(const FInputActionValue& InputActionValue)
{
	ToggleWeapon(EUpEquipmentSlot::Weapon1);
}

void AUpPlayerController::ToggleWeapon2(const FInputActionValue& InputActionValue)
{
	ToggleWeapon(EUpEquipmentSlot::Weapon2);
}

void AUpPlayerController::ToggleWeapon(const EUpEquipmentSlot::Type EquipmentSlot) const
{
	if (const auto CharacterEquippable = Cast<IUpCharacterEquippable>(PossessedCharacter))
	{
		if (const auto& EquipmentSlotData = CharacterEquippable->GetCharacterEquipment().GetEquipmentSlotData(EquipmentSlot);
			EquipmentSlotData.IsValid())
		{
			if (EquipmentSlotData.bActivated)
			{
				CharacterEquippable->DeactivateEquipment(EquipmentSlot);
			} else
			{
				CharacterEquippable->ActivateEquipment(EquipmentSlot, EquipmentSlotData);
			}
		}
	}
}

void AUpPlayerController::ToggleCrouch(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (PossessedCharacter->bIsCrouched)
	{
		PossessedCharacter->UnCrouch();
	} else
	{
		PossessedCharacter->Crouch();
	}
}

void AUpPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	PossessedCharacter->Jump();
}

void AUpPlayerController::StartSprint(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	if (const auto MovementComponent = PossessedCharacter->GetCustomMovementComponent())
	{
		MovementComponent->ToggleSprint(true);
	}
}

void AUpPlayerController::StopSprint(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	if (const auto MovementComponent = PossessedCharacter->GetCustomMovementComponent())
	{
		MovementComponent->ToggleSprint(false);
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
	if (!PossessedCharacter) return;
	
	const auto InputActionVector = InputActionValue.Get<FVector2D>();
	const FRotationMatrix RotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f));

	PossessedCharacter->AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::X), InputActionVector.Y);
	PossessedCharacter->AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::Y), InputActionVector.X);
}

void AUpPlayerController::StartAimingGun(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("[temp] start aim gun"))
}

void AUpPlayerController::StopAimingGun(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("[temp] stop aim gun"))
}

void AUpPlayerController::StartFiringGun(const FInputActionValue& InputActionValue)
{
	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(PossessedCharacter))
	{
		if (const auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			FGameplayTagContainer AbilityTags;
			AbilityTags.AddTag(TAG_Combat_Gun_Fire);
			
			AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
		}
	}
}

void AUpPlayerController::StopFiringGun(const FInputActionValue& InputActionValue)
{
	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(PossessedCharacter))
	{
		if (const auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			FGameplayTagContainer AbilityTags;
			AbilityTags.AddTag(TAG_Combat_Gun_Fire);
			
			AbilitySystemComponent->CancelAbilities(&AbilityTags);
		}
	}
}
