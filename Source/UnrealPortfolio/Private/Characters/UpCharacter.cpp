// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "GameplayEffect.h"
#include "UpGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpInventoryComponent.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpCharacter::AUpCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	if (const auto CapsuleComponent = GetCapsuleComponent())
	{
		CapsuleComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	if (const auto Mesh = GetMesh())
	{
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
		Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(InitPrimaryAttributesEffectClass);
	check(InitVitalAttributesEffectClass);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());
}

void AUpCharacter::SetYaw(const float InYaw)
{
	SetActorRotation(FRotator(0.f, InYaw, 0.f));
}

void AUpCharacter::SetRootMotionTargetLocation(const FVector& InRootMotionTargetLocation)
{
	RootMotionTargetLocation = InRootMotionTargetLocation;
	bHasRootMotionTargetLocation = true;
}

void AUpCharacter::UnsetRootMotionTargetLocation()
{
	RootMotionTargetLocation = FVector();
	bHasRootMotionTargetLocation = false;
}

void AUpCharacter::EquipItem(const FUpItemInstance& ItemInstance)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto ItemData = GameInstance->GetItemData(ItemInstance.ItemTagId); ItemData.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Equip item %s"), *ItemData.Name.ToString())

			if (ArmingState == EUpCharacterArmingState::ArmedPistol)
			{
				ArmingState = EUpCharacterArmingState::Unarmed;
			} else
			{
				ArmingState = EUpCharacterArmingState::ArmedPistol;
			}
		}
	}
}
