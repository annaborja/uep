// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "GameplayEffect.h"
#include "UpGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpCombatComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GAS/Attributes/UpHealthAttributeSet.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "Utils/Constants.h"
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
		CapsuleComponent->SetCollisionResponseToChannel(TRACE_CHANNEL_WEAPON, ECR_Ignore);
	}

	if (const auto Mesh = GetMesh())
	{
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
		Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		
		Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(TRACE_CHANNEL_WEAPON, ECR_Block);
	}
	
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CombatComponent = CreateDefaultSubobject<UUpCombatComponent>(TEXT("CombatComponent"));
	
	HealthAttributeSet = CreateDefaultSubobject<UUpHealthAttributeSet>(TEXT("HealthAttributeSet"));
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HitReactionsMontage_ThirdPerson);
	check(InitHealthAttributesEffectClass);
	check(InitPrimaryAttributesEffectClass);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());

	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayEffect>> InitAttributesEffectClasses;
		
		if (InitHealthAttributesEffectClass) InitAttributesEffectClasses.Add(InitHealthAttributesEffectClass);
		if (InitPrimaryAttributesEffectClass) InitAttributesEffectClasses.Add(InitPrimaryAttributesEffectClass);
		
		AbilitySystemComponent->Init(this, this, InitAttributesEffectClasses, TArray<TSubclassOf<UGameplayAbility>> {});
	}
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

TArray<UUpAttributeSet*> AUpCharacter::GetAttributeSets() const
{
	return TArray<UUpAttributeSet*> { HealthAttributeSet, PrimaryAttributeSet };
}

void AUpCharacter::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot, const FUpEquipmentSlotData& EquipmentSlotData)
{
	DeactivateEquipment(EquipmentSlot);
	Equipment.ActivateEquipmentSlot(EquipmentSlot);
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto& ItemData = GameInstance->GetItemData(EquipmentSlotData.ItemInstance.ItemTagId); ItemData.IsValid())
		{
			if (ItemData.ResultingPosture != EUpCharacterPosture::Casual)
			{
				Posture = ItemData.ResultingPosture;
			}

			if (ItemData.StaticMeshActorClass)
			{
				if (const auto World = GetWorld())
				{
					if (const auto Mesh = GetMesh())
					{
						const auto SpawnLocation = GetActorLocation();
						const auto SpawnRotation= GetActorRotation();
				
						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = this;

						if (ItemData.ItemCategory == EUpItemCategory::Weapon)
						{
							WeaponActor = Cast<AStaticMeshActor>(World->SpawnActor(ItemData.StaticMeshActorClass, &SpawnLocation, &SpawnRotation, SpawnParams));
					
							if (IsValid(WeaponActor))
							{
								WeaponActor->SetActorEnableCollision(false);
								WeaponActor->AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false),
									ItemData.SocketTag.IsValid() ? ItemData.SocketTag.GetTagName() : NAME_None);
							}
						}
					}
				}
			}

			if (AbilitySystemComponent)
			{
				for (const auto& AbilityGrantSpec : ItemData.AbilityGrantSpecs)
				{
					if (!AbilityGrantSpec.IsValid()) continue;
					
					AbilitySystemComponent->GrantAbility(AbilityGrantSpec.AbilityClass);
				}
			}
			
			OnEquipmentActivation(ItemData);
		}
	}
}

void AUpCharacter::DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	Equipment.DeactivateEquipmentSlot(EquipmentSlot);
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot); EquipmentSlotData.IsValid() && EquipmentSlotData.bActivated)
		{
			if (const auto& ItemData = GameInstance->GetItemData(EquipmentSlotData.ItemInstance.ItemTagId); ItemData.IsValid())
			{
				if (ItemData.ResultingPosture != EUpCharacterPosture::Casual)
				{
					Posture = EUpCharacterPosture::Casual;
				}

				if (AbilitySystemComponent)
				{
					for (const auto& AbilityGrantSpec : ItemData.AbilityGrantSpecs)
					{
						if (!AbilityGrantSpec.IsValid()) continue;

						if (AbilityGrantSpec.GrantDuration == EUpAbilityGrantDuration::WhileEquipped)
						{
							AbilitySystemComponent->RevokeAbility(AbilityGrantSpec.AbilityClass);
						}
					}
				}

				if (ItemData.ItemCategory == EUpItemCategory::Weapon)
				{
					if (IsValid(WeaponActor))
					{
						WeaponActor->Destroy();
						WeaponActor = nullptr;
					}
				}
				
				OnEquipmentDeactivation(ItemData);
			}
		}
	}
}
