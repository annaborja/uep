// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableCharacter.h"

#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Attributes/UpAmmoAttributeSet.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "Items/UpAmmo.h"
#include "Levels/UpLevelScriptActor.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void AUpPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(AnimClass_FirstPerson);
	check(AnimClass_ThirdPerson);
	check(SkeletalMesh_FirstPerson);
	check(SkeletalMesh_ThirdPerson);

	check(ClimbingMontage_FirstPerson);
	check(MantlesMontage_FirstPerson);
	check(ReloadsMontage_FirstPerson);

	CapsuleHitDelegate.BindUFunction(this, FName(TEXT("OnCapsuleHit")));

	if (const auto Capsule = GetCapsuleComponent())
	{
		Capsule->OnComponentHit.Add(CapsuleHitDelegate);
	}
	
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

UAnimMontage* AUpPlayableCharacter::GetClimbingMontage() const
{
	if (IsInFirstPersonMode()) return ClimbingMontage_FirstPerson;
	
	return Super::GetClimbingMontage();
}

UAnimMontage* AUpPlayableCharacter::GetMantlesMontage() const
{
	if (IsInFirstPersonMode()) return MantlesMontage_FirstPerson;
	
	return Super::GetMantlesMontage();
}

UAnimMontage* AUpPlayableCharacter::GetReloadsMontage() const
{
	if (IsInFirstPersonMode()) return ReloadsMontage_FirstPerson;
	
	return Super::GetReloadsMontage();
}

bool AUpPlayableCharacter::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	if (bDebugTagSpecGrant)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrantTagSpec %s"), *TagSpec.ToString())
	}
	
	bool bSuccess = false;
		
	if (TagSpec.Tag.MatchesTag(TAG_Item_Ammo))
	{
		bSuccess = AUpAmmo::HandleAmmoTagSpecGrant(this, TagSpec);
	} else if (TagSpec.Count > 0)
	{
		// bSuccess = GameInstance->AddPlayerCharacterTag(TagSpec.Tag);
	} else if (TagSpec.Count < 0)
	{
		// bSuccess = GameInstance->RemovePlayerCharacterTag(TagSpec.Tag);
	}

	if (bSuccess)
	{
		// UUpPlayerPartyComponent::OnPlayerTagSpecGranted(WorldContextObject, TagSpec);
	}

	return bSuccess;
}

void AUpPlayableCharacter::ActivateCameraView(const EUpCameraView::Type CameraViewType)
{
	switch (CameraViewType)
	{
	case EUpCameraView::FirstPerson:
		SetUpFirstPersonMesh();
		SetUpFirstPersonController();

		if (CameraSpringArm)
		{
			CameraSpringArm->TargetArmLength = 5.f;
			CameraSpringArm->SocketOffset = FVector(10.f, 0.f, 80.f);
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
		SetUpThirdPersonController();
		SetUpThirdPersonCamera();

		break;
	case EUpCameraView::ThirdPerson_OverTheShoulder:
		SetUpThirdPersonMesh();
		SetUpFirstPersonController();

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
		SetUpFirstPersonController();
		SetUpThirdPersonCamera();
		
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid camera view %d"), CameraViewType)
	}
	
	if (CustomPlayerController) CustomPlayerController->SetCameraView(CameraViewType);
	
	if (CustomMovementComponent) CustomMovementComponent->HandleCameraViewChange();
}

void AUpPlayableCharacter::OnItemEquip(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot)
{
	Super::OnItemEquip(ItemActor, EquipmentSlot);

	if (const auto CustomController = GetCustomPlayerController())
	{
		if (const auto CustomHud = CustomController->GetCustomHud())
		{
			CustomHud->BroadcastEquipmentUpdate(EquipmentSlot, ItemActor->GetItemData());
		}
	}
}

void AUpPlayableCharacter::OnItemUnequip(const EUpEquipmentSlot::Type EquipmentSlot)
{
	Super::OnItemUnequip(EquipmentSlot);

	if (const auto CustomController = GetCustomPlayerController())
	{
		if (const auto CustomHud = CustomController->GetCustomHud())
		{
			CustomHud->BroadcastEquipmentUpdate(EquipmentSlot, FUpItemData());
		}
	}
}

void AUpPlayableCharacter::OnEquipmentActivation(const EUpEquipmentSlot::Type EquipmentSlot)
{
	Super::OnEquipmentActivation(EquipmentSlot);

	if (bIsPlayer)
	{
		if (const auto CustomController = GetCustomPlayerController())
		{
			if (const auto CustomHud = CustomController->GetCustomHud())
			{
				CustomHud->BroadcastEquipmentActivationUpdate(EquipmentSlot, true);
			}
		}
		
		if (FUpCharacterEquipment::IsWeaponSlot(EquipmentSlot))
		{
			if (CustomPlayerController) CustomPlayerController->ResetInputMappingContexts();
			
			HandleWeaponDelegates(Cast<AUpWeapon>(Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor));
		}
	}
}

void AUpPlayableCharacter::OnEquipmentDeactivation(const EUpEquipmentSlot::Type EquipmentSlot)
{
	Super::OnEquipmentDeactivation(EquipmentSlot);

	if (bIsPlayer)
	{
		if (const auto CustomController = GetCustomPlayerController())
		{
			if (const auto CustomHud = CustomController->GetCustomHud())
			{
				CustomHud->BroadcastEquipmentActivationUpdate(EquipmentSlot, false);
			}
		}
		
		if (FUpCharacterEquipment::IsWeaponSlot(EquipmentSlot))
		{
			if (CustomPlayerController) CustomPlayerController->ResetInputMappingContexts();

			HandleWeaponDelegates(nullptr);
		}
	}
}

void AUpPlayableCharacter::OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsPlayer()) return;

	if (const auto AllyNpc = Cast<AUpPlayableCharacter>(OtherActor))
	{
		if (const auto AiController = Cast<AUpAiController>(AllyNpc->GetController()))
		{
			if (const auto Blackboard = AiController->GetBlackboardComponent())
			{
				Blackboard->SetValueAsBool(FName(TEXT("NeedsPlayerBuffer")), true);
			}
		}
		AllyNpc->AddMovementInput(Hit.ImpactNormal, -1.f);
	}
}

void AUpPlayableCharacter::InitForPlayer()
{
	bIsPlayer = true;
	
	SetRelaxed(false);
	
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
	
	if (CustomPlayerController)
	{
		ActivateCameraView(CustomPlayerController->GetCameraView());

		if (const auto CustomHud = CustomPlayerController->GetCustomHud())
		{
			CustomHud->BroadcastPossessedCharacter(this);

			if (const auto& EquipmentSlotData = Equipment.GetPotentialActiveWeaponSlotData(); EquipmentSlotData.bActivated)
			{
				HandleWeaponDelegates(Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor));
			} else
			{
				HandleWeaponDelegates(nullptr);
			}

			for (const auto EquipmentSlot : TArray { EUpEquipmentSlot::Weapon1, EUpEquipmentSlot::Weapon2, EUpEquipmentSlot::Item1, EUpEquipmentSlot::Item2 })
			{
				const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);
				
				CustomHud->BroadcastEquipmentUpdate(EquipmentSlot, EquipmentSlotData.ItemInstance.ItemActor ? EquipmentSlotData.ItemInstance.ItemActor->GetItemData() : FUpItemData());
				CustomHud->BroadcastEquipmentActivationUpdate(EquipmentSlot, EquipmentSlotData.bActivated);
			}
		}

		if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
		{
			// Set level script interaction data (if any).
			if (const auto LevelScriptActor = GameInstance->GetLevelScriptActor())
			{
				if (const auto LevelScriptInteractionData = LevelScriptActor->GetInteractionData(CustomPlayerController); LevelScriptInteractionData.Interactable)
				{
					PlayerInteractionComponent->SetInteractionData(LevelScriptInteractionData);
				}
			}
		}
	}
}

void AUpPlayableCharacter::TearDownForPlayer()
{
	if (CustomPlayerController)
	{
		if (!CustomPlayerController->IsDebugCameraActive())
		{
			SetUpThirdPersonMesh();
			SetUpThirdPersonController();
		}
	}

	// Make sure the UI doesn't display a stale interaction prompt.
	if (PlayerInteractionComponent) PlayerInteractionComponent->SetInteractionData(FUpInteractionData());
	
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

	if (AutoPossessAI == EAutoPossessAI::PlacedInWorldOrSpawned || AutoPossessAI == EAutoPossessAI::Spawned)
	{
		if (const auto World = GetWorld())
		{
			if (const auto AiController = World->SpawnActor<AUpAiController>(
				AUpAiController::StaticClass(), GetActorLocation(), GetActorRotation(), FActorSpawnParameters()))
			{
				AiController->Possess(this);
			}
		}
	}
}

void AUpPlayableCharacter::SetUpFirstPersonController()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
}

void AUpPlayableCharacter::SetUpFirstPersonMesh() const
{
	if (const auto Mesh = GetMesh())
	{
		Mesh->SetSkeletalMesh(SkeletalMesh_FirstPerson);
		Mesh->SetAnimClass(AnimClass_FirstPerson);
		Mesh->SetCastShadow(false);
	}

	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		if (const auto ItemActor = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor)
		{
			ItemActor->ToggleCastShadows(false);
		}
	}
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

void AUpPlayableCharacter::SetUpThirdPersonController()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AUpPlayableCharacter::SetUpThirdPersonMesh() const
{
	if (const auto Mesh = GetMesh())
	{
		Mesh->SetSkeletalMesh(SkeletalMesh_ThirdPerson);
		Mesh->SetAnimClass(AnimClass_ThirdPerson);
		Mesh->SetCastShadow(true);
	}

	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		if (const auto ItemActor = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor)
		{
			ItemActor->ToggleCastShadows(true);
		}
	}
}

void AUpPlayableCharacter::HandleWeaponDelegates(AUpWeapon* Weapon)
{
	if (ActiveWeapon)
	{
		if (const auto WeaponAbilitySystemComponent = ActiveWeapon->GetAbilitySystemComponent())
		{
			if (const auto AttributeSet = ActiveWeapon->GetAmmoAttributeSet())
			{
				for (const auto TagAttributeMapping : AttributeSet->GetTagAttributeMap())
				{
					if (const auto DelegateHandle = AttributeValueDelegateHandleMap.Find(TagAttributeMapping.Key))
					{
						WeaponAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TagAttributeMapping.Value()).Remove(*DelegateHandle);
					}
				}
			}	
		}
	}

	ActiveWeapon = Weapon;
	
	if (!CustomPlayerController) return;

	if (const auto CustomHud = CustomPlayerController->GetCustomHud())
	{
		CustomHud->BroadcastActiveWeapon(Weapon);

		if (Weapon)
		{
			if (const auto WeaponAbilitySystemComponent = Weapon->GetAbilitySystemComponent())
			{
				if (const auto AttributeSet = Weapon->GetAmmoAttributeSet())
				{
					for (const auto TagAttributeMapping : AttributeSet->GetTagAttributeMap())
					{
						AttributeValueDelegateHandleMap.Add(TagAttributeMapping.Key, WeaponAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TagAttributeMapping.Value())
							.AddLambda([this, AttributeSet, CustomHud, TagAttributeMapping, Weapon](const FOnAttributeChangeData& Data)
							{
								if (IsValid(CustomHud))
								{
									CustomHud->BroadcastAttributeValue(Weapon->GetTagId(), TagAttributeMapping.Key, TagAttributeMapping.Value(), AttributeSet);
								}
							}));
					}
				}
			}
		}
	}
}
