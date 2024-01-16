// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/UpPlayerCameraManager.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "Interfaces/UpInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tags/CombatTags.h"
#include "UI/UpHud.h"

AUpPlayerController::AUpPlayerController(): APlayerController()
{
	PlayerCameraManagerClass = AUpPlayerCameraManager::StaticClass();
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
	
	if (InputMappingContext_CharacterSwitcher) DeactivateInputMappingContext(InputMappingContext_CharacterSwitcher);
	ResetInputMappingContexts();
}

void AUpPlayerController::SwitchCharacter(AUpPlayableNpc* Npc)
{
	CloseCharacterSwitcher();
	Possess(Npc);
}

void AUpPlayerController::ActivateInputMappingContext(const UInputMappingContext* InputMappingContext, const bool bClearExisting, const int32 Priority) const
{
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bClearExisting)
		{
			if (bDebugInputMappingContexts)
			{
				UE_LOG(LogTemp, Warning, TEXT("Clear all input mapping contexts"))
			}
			
			Subsystem->ClearAllMappings();
		}
		
		if (bDebugInputMappingContexts)
		{
			UE_LOG(LogTemp, Warning, TEXT("Add input mapping context %s with priority %d"), *InputMappingContext->GetName(), Priority)
		}
		
		Subsystem->AddMappingContext(InputMappingContext, Priority);
	}
}

void AUpPlayerController::DeactivateInputMappingContext(const UInputMappingContext* InputMappingContext) const
{
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bDebugInputMappingContexts)
		{
			UE_LOG(LogTemp, Warning, TEXT("Remove input mapping context %s"), *InputMappingContext->GetName())
		}
		
		Subsystem->RemoveMappingContext(InputMappingContext);
	}
}

void AUpPlayerController::ResetInputMappingContexts() const
{
	if (bDebugInputMappingContexts)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetInputMappingContexts %s"), PossessedCharacter ? *PossessedCharacter->GetName() : *GetName())
	}
	
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext_Base && !Subsystem->HasMappingContext(InputMappingContext_Base))
		{
			constexpr auto Priority = 0;
			
			if (bDebugInputMappingContexts)
			{
				UE_LOG(LogTemp, Warning, TEXT("Add input mapping context %s with priority %d"), *InputMappingContext_Base->GetName(), Priority)
			}
			
			Subsystem->AddMappingContext(InputMappingContext_Base, Priority);
		}
		
		if (PossessedCharacter)
		{
			if (InputMappingContext_Gun)
			{
				if (const auto& Equipment = PossessedCharacter->GetCharacterEquipment();
					Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon1).bActivated || Equipment.GetEquipmentSlotData(EUpEquipmentSlot::Weapon2).bActivated)
				{
					if (!Subsystem->HasMappingContext(InputMappingContext_Gun))
					{
						constexpr auto Priority = 1;
						
						if (bDebugInputMappingContexts)
						{
							UE_LOG(LogTemp, Warning, TEXT("Add input mapping context %s with priority %d"), *InputMappingContext_Gun->GetName(), Priority)
						}
						
						Subsystem->AddMappingContext(InputMappingContext_Gun, Priority);
					}
				} else
				{
					if (bDebugInputMappingContexts)
					{
						UE_LOG(LogTemp, Warning, TEXT("Remove input mapping context %s"), *InputMappingContext_Gun->GetName())
					}
					
					Subsystem->RemoveMappingContext(InputMappingContext_Gun);
				}
			}
		}
	}
}

void AUpPlayerController::CreateTemporaryCamera(const AActor* LookTarget, const float CameraBlendTime, const float AspectRatio, const float FieldOfView)
{
	if (!PossessedCharacter) return;

	if (TemporaryCamera) DestroyTemporaryCamera(CameraBlendTime);
	
	ActivateInputMappingContext(InputMappingContext_InteractionOnly);

	if (const auto PlayerCameraComponent = PossessedCharacter->GetCameraComponent())
	{
		if (const auto World = GetWorld())
		{
			const auto CameraLocation = PlayerCameraComponent->GetComponentLocation();
			const auto CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, LookTarget->GetActorLocation());

			TemporaryCamera = Cast<ACameraActor>(World->SpawnActor(ACameraActor::StaticClass(), &CameraLocation, &CameraRotation));

			if (TemporaryCamera)
			{
				if (const auto DialogueCameraComponent = TemporaryCamera->GetCameraComponent())
				{
					DialogueCameraComponent->SetAspectRatio(AspectRatio);
					DialogueCameraComponent->SetFieldOfView(FieldOfView);
				}
			
				SetViewTargetWithBlend(TemporaryCamera, CameraBlendTime);
			}
		}
	}
}

void AUpPlayerController::DestroyTemporaryCamera(const float CameraBlendTime)
{
	if (!IsValid(TemporaryCamera)) return;

	if (PossessedCharacter)
	{
		SetViewTargetWithBlend(PossessedCharacter, CameraBlendTime, VTBlend_Linear, 0.f, true);
	}

	if (const auto World = GetWorld())
	{
		World->DestroyActor(TemporaryCamera);
		TemporaryCamera = nullptr;
	}

	ResetInputMappingContexts();
}

void AUpPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(InputMappingContext_Base);
	check(InputMappingContext_CharacterSwitcher);
	check(InputMappingContext_Gun);
	check(InputMappingContext_InteractionOnly);

	check(AimGunInputAction);
	check(CloseCharacterSwitcherInputAction);
	check(CrouchInputAction);
	check(FireWeaponInputAction);
	check(InteractInputAction);
	check(JumpInputAction);
	check(LookInputAction);
	check(MoveInputAction);
	check(NavigateCharacterSwitcherInputAction);
	check(OpenCharacterSwitcherInputAction);
	check(PauseGameInputAction);
	check(ReloadInputAction);
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

	if (bDebugPossession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Possess %s"), *InPawn->GetName())
	}
	
	PossessedCharacter = Cast<AUpPlayableCharacter>(InPawn);
	ResetInputMappingContexts();
}

void AUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(ToggleCameraViewInputAction, ETriggerEvent::Started, this, &ThisClass::ToggleCameraView);
	EnhancedInputComponent->BindAction(ToggleDebugCameraInputAction, ETriggerEvent::Started, this, &ThisClass::ToggleDebugCamera);
	
	EnhancedInputComponent->BindAction(PauseGameInputAction, ETriggerEvent::Completed, this, &ThisClass::PauseGame);
	EnhancedInputComponent->BindAction(ReloadInputAction, ETriggerEvent::Triggered, this, &ThisClass::Reload);
	
	EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Started, this, &ThisClass::StartInteraction);
	EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Completed, this, &ThisClass::EndInteraction);
	
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
	
	EnhancedInputComponent->BindAction(FireWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::StartFiringWeapon);
	EnhancedInputComponent->BindAction(FireWeaponInputAction, ETriggerEvent::Completed, this, &ThisClass::StopFiringWeapon);
}

void AUpPlayerController::ToggleCameraView(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	switch (CameraView)
	{
	case EUpCameraView::FirstPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson_OverTheShoulder);
		break;
	case EUpCameraView::ThirdPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson);
		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson);
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

void AUpPlayerController::Reload(const FInputActionValue& InputActionValue)
{
	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(PossessedCharacter))
	{
		if (const auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			FGameplayTagContainer AbilityTags;
			AbilityTags.AddTag(TAG_Combat_Reload);
			
			AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
		}
	}
}

void AUpPlayerController::StartInteraction(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto InteractionComponent = PossessedCharacter->GetPlayerInteractionComponent())
	{
		if (const auto TargetInteractable = InteractionComponent->GetTargetInteractable())
		{
			if (const auto Interactable = Cast<IUpInteractable>(TargetInteractable))
			{
				ActiveInteractable = TargetInteractable;
				Interactable->Interact(this);
			}
		}
	}
}

void AUpPlayerController::EndInteraction(const FInputActionValue& InputActionValue)
{
	if (const auto Interactable = Cast<IUpInteractable>(ActiveInteractable))
	{
		Interactable->OnInteractionEnd(this);
	}
}

void AUpPlayerController::OpenCharacterSwitcher(const FInputActionValue& InputActionValue)
{
	if (!CustomHud) return;

	CustomHud->OpenCharacterSwitcher();
	EnableMouse();

	if (InputMappingContext_CharacterSwitcher) ActivateInputMappingContext(InputMappingContext_CharacterSwitcher);
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
	if (!PossessedCharacter) return;

	if (PossessedCharacter->GetCharacterEquipment().GetEquipmentSlotData(EquipmentSlot).bActivated)
	{
		PossessedCharacter->DeactivateEquipment(EquipmentSlot);
	} else
	{
		PossessedCharacter->ActivateEquipment(EquipmentSlot);
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
	
	if (const auto CustomMovementComponent = PossessedCharacter->GetCustomMovementComponent();
		CustomMovementComponent && CustomMovementComponent->IsClimbing())
	{
		HandleClimbingMovement(InputActionValue);
	} else
	{
		HandleGroundMovement(InputActionValue);
	}
}

void AUpPlayerController::HandleGroundMovement(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	const auto InputActionVector = InputActionValue.Get<FVector2D>();
	const FRotationMatrix RotationMatrix(FRotator(0.0, GetControlRotation().Yaw, 0.0));

	PossessedCharacter->AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::X), InputActionVector.Y);
	PossessedCharacter->AddMovementInput(RotationMatrix.GetUnitAxis(EAxis::Y), InputActionVector.X);
}

void AUpPlayerController::HandleClimbingMovement(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto CustomMovementComponent = PossessedCharacter->GetCustomMovementComponent())
	{
		const auto InputActionVector = InputActionValue.Get<FVector2D>();
		const auto ClimbedSurfaceNormal = CustomMovementComponent->GetClimbedSurfaceNormal();

		PossessedCharacter->AddMovementInput(
			FVector::CrossProduct(-ClimbedSurfaceNormal, PossessedCharacter->GetActorRightVector()), InputActionVector.Y);
		PossessedCharacter->AddMovementInput(
			FVector::CrossProduct(-ClimbedSurfaceNormal,-PossessedCharacter->GetActorUpVector()), InputActionVector.X);
	}
}

void AUpPlayerController::StartAimingGun(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("[temp] start aim gun"))
}

void AUpPlayerController::StopAimingGun(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("[temp] stop aim gun"))
}

void AUpPlayerController::StartFiringWeapon(const FInputActionValue& InputActionValue)
{
	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(PossessedCharacter))
	{
		if (const auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			FGameplayTagContainer AbilityTags;
			AbilityTags.AddTag(TAG_Combat_Attack_Gun_Fire);
			
			AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
		}
	}
}

void AUpPlayerController::StopFiringWeapon(const FInputActionValue& InputActionValue)
{
	if (const auto AbilitySystemInterface = Cast<IAbilitySystemInterface>(PossessedCharacter))
	{
		if (const auto AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			FGameplayTagContainer AbilityTags;
			AbilityTags.AddTag(TAG_Combat_Attack_Gun_Fire);

			AbilitySystemComponent->CancelAbilities(&AbilityTags);
		}
	}
}
