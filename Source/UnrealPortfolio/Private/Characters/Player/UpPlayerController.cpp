// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
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
#include "Tags/GasTags.h"
#include "UI/UpHud.h"
#include "Utils/Constants.h"

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

bool AUpPlayerController::ProjectReticleToWorld(FVector& WorldPosition, FVector& WorldDirection) const
{
	if (!GEngine || !GEngine->GameViewport) return false;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	return UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(ViewportSize.X / 2.f, ViewportSize.Y / 2.f),
		WorldPosition, WorldDirection);
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
				if (const auto& Equipment = PossessedCharacter->GetEquipment();
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

	check(InputAction_ToggleDebugCamera);
	check(InputAction_Move);
	check(InputAction_Sprint);
	check(InputAction_Look);
	check(InputAction_SwitchCameraView);
	check(InputAction_Jump);
	check(InputAction_Reload);
	check(InputAction_Interact);
	check(InputAction_Weapon1);
	check(InputAction_Weapon2);
	check(InputAction_AimGun);
	check(InputAction_FireWeapon);

	CustomHud = CastChecked<AUpHud>(GetHUD());
	CustomHud->Init(this);
}

void AUpPlayerController::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!PossessedCharacter) return;

	if (TargetCameraFov >= 0.f)
	{
		if (const auto Camera = PossessedCharacter->GetCameraComponent())
		{
			if (FMath::IsNearlyEqual(Camera->FieldOfView, TargetCameraFov))
			{
				TargetCameraFov = -1.f;
			} else
			{
				Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetCameraFov, DeltaSeconds, InterpSpeed_CameraFov));
			}
		}
	}

	AimAssistLevel = EUpAimAssistLevel::None;
	FVector ReticleWorldPosition;
	FVector ReticleWorldDirection;

	if (const auto Weapon = PossessedCharacter->GetActiveWeapon();
		Weapon && PossessedCharacter->CanShoot() && ProjectReticleToWorld(ReticleWorldPosition, ReticleWorldDirection))
	{
		const auto LineTraceStart = ReticleWorldPosition;
		TArray<AActor*> ActorsToIgnore { PossessedCharacter };

		for (const auto Actor : PossessedCharacter->Children)
		{
			ActorsToIgnore.Add(Actor);
		}
		
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(this, LineTraceStart, LineTraceStart + ReticleWorldDirection * Weapon->GetRange(),
			UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_AIM_ASSIST), false, ActorsToIgnore,
			bDebugAimAssist ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, HitResult, true);

		if (HitResult.bBlockingHit)
		{
			AimAssistLevel = EUpAimAssistLevel::Medium;
		}
	}
}

void AUpPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (bDebugPossession)
	{
		UE_LOG(LogTemp, Warning, TEXT("Possess %s"), *InPawn->GetName())
	}
	
	PossessedCharacter = CastChecked<AUpPlayableCharacter>(InPawn);
	ResetInputMappingContexts();
}

void AUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(InputAction_ToggleDebugCamera, ETriggerEvent::Started, this, &ThisClass::ToggleDebugCamera);
	
	EnhancedInputComponent->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	
	EnhancedInputComponent->BindAction(InputAction_Sprint, ETriggerEvent::Started, this, &ThisClass::StartSprint);
	EnhancedInputComponent->BindAction(InputAction_Sprint, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
	
	EnhancedInputComponent->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	
	EnhancedInputComponent->BindAction(InputAction_SwitchCameraView, ETriggerEvent::Triggered, this, &ThisClass::SwitchCameraView);
	
	EnhancedInputComponent->BindAction(InputAction_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
	
	EnhancedInputComponent->BindAction(InputAction_Reload, ETriggerEvent::Triggered, this, &ThisClass::Reload);
	
	EnhancedInputComponent->BindAction(InputAction_Interact, ETriggerEvent::Started, this, &ThisClass::StartInteraction);
	EnhancedInputComponent->BindAction(InputAction_Interact, ETriggerEvent::Completed, this, &ThisClass::EndInteraction);
	
	EnhancedInputComponent->BindAction(InputAction_Weapon1, ETriggerEvent::Started, this, &ThisClass::ToggleWeapon1);
	EnhancedInputComponent->BindAction(InputAction_Weapon2, ETriggerEvent::Started, this, &ThisClass::ToggleWeapon2);
	
	EnhancedInputComponent->BindAction(InputAction_AimGun, ETriggerEvent::Started, this, &ThisClass::StartAimingGun);
	EnhancedInputComponent->BindAction(InputAction_AimGun, ETriggerEvent::Completed, this, &ThisClass::StopAimingGun);
	
	EnhancedInputComponent->BindAction(InputAction_FireWeapon, ETriggerEvent::Started, this, &ThisClass::StartFiringWeapon);
	EnhancedInputComponent->BindAction(InputAction_FireWeapon, ETriggerEvent::Completed, this, &ThisClass::StopFiringWeapon);
	
	EnhancedInputComponent->BindAction(InputAction_PauseGame, ETriggerEvent::Completed, this, &ThisClass::PauseGame);
	
	EnhancedInputComponent->BindAction(InputAction_OpenCharacterSwitcher, ETriggerEvent::Triggered, this, &ThisClass::OpenCharacterSwitcher);
	EnhancedInputComponent->BindAction(InputAction_CloseCharacterSwitcher, ETriggerEvent::Triggered, this, &ThisClass::TriggerCloseCharacterSwitcher);
	EnhancedInputComponent->BindAction(InputAction_NavigateCharacterSwitcher, ETriggerEvent::Triggered, this, &ThisClass::NavigateCharacterSwitcher);
	
	EnhancedInputComponent->BindAction(InputAction_Crouch, ETriggerEvent::Started, this, &ThisClass::ToggleCrouch);
}

void AUpPlayerController::ToggleDebugCamera(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	switch (CameraView)
	{
	case EUpCameraView::FirstPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson_Debug);
		break;
	case EUpCameraView::FirstPerson_Debug:
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson);
		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson_OverTheShoulder_Debug);
		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder_Debug:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson);
		break;
	case EUpCameraView::ThirdPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson_OverTheShoulder);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid camera view type %d"), CameraView)
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

void AUpPlayerController::StartSprint(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_Sprint);
		
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
	}
}

void AUpPlayerController::StopSprint(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_Sprint);
		
		AbilitySystemComponent->CancelAbilities(&AbilityTags);
	}
}

void AUpPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const auto InputActionVector = InputActionValue.Get<FVector2D>();
	auto InputMultiplier = 1.f;

	if (PossessedCharacter)
	{
		if (PossessedCharacter->IsAiming())
		{
			InputMultiplier *= LookInputMultiplier_Aiming;
		}

		if (AimAssistLevel == EUpAimAssistLevel::Medium)
		{
			InputMultiplier *= LookInputMultiplier_AimAssist;
		}
	}

	if (bDebugAimAssist)
	{
		UE_LOG(LogTemp, Warning, TEXT("Look input multiplier: %g"), InputMultiplier)
	}

	AddPitchInput(InputActionVector.Y * InputMultiplier);
	AddYawInput(InputActionVector.X * InputMultiplier);
}

void AUpPlayerController::SwitchCameraView(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;
	
	switch (CameraView)
	{
	case EUpCameraView::FirstPerson:
		PossessedCharacter->ActivateCameraView(EUpCameraView::ThirdPerson_OverTheShoulder);
		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder:
		PossessedCharacter->ActivateCameraView(EUpCameraView::FirstPerson);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid camera view type %d"), CameraView)
		break;
	}
}

void AUpPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	PossessedCharacter->Jump();
}

void AUpPlayerController::Reload(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_GunReload);
		
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
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

	FGameplayEventData EventPayload;
	EventPayload.EventMagnitude = EquipmentSlot;
		
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PossessedCharacter, TAG_Ability_ActivateEquipment, EventPayload);
	
	// if (PossessedCharacter->GetEquipment().GetEquipmentSlotData(EquipmentSlot).bActivated)
	// {
		// PossessedCharacter->DeactivateEquipment(EquipmentSlot);
	// } else
	// {
		// PossessedCharacter->ActivateEquipment(EquipmentSlot);
	// }
}

void AUpPlayerController::StartAimingGun(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_AimDownSights);
		
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
	}
}

void AUpPlayerController::StopAimingGun(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_AimDownSights);
		
		AbilitySystemComponent->CancelAbilities(&AbilityTags);
	}
}

void AUpPlayerController::StartFiringWeapon(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_GunFire);
		
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
	}
}

void AUpPlayerController::StopFiringWeapon(const FInputActionValue& InputActionValue)
{
	if (!PossessedCharacter) return;

	if (const auto AbilitySystemComponent = PossessedCharacter->GetAbilitySystemComponent())
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_GunFire);
		
		AbilitySystemComponent->CancelAbilities(&AbilityTags);
	}
}

void AUpPlayerController::PauseGame(const FInputActionValue& InputActionValue)
{
	if (CustomHud) CustomHud->OpenMainMenu();
	
	UGameplayStatics::SetGamePaused(this, true);
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
