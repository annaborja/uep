// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "GameplayEffect.h"
#include "UpGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpCombatComponent.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Items/UpWeapon.h"
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
	
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));
	VitalAttributeSet = CreateDefaultSubobject<UUpVitalAttributeSet>(TEXT("VitalAttributeSet"));
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HitReactionsMontage_ThirdPerson);
	
	check(InitPrimaryAttributesEffectClass);
	check(InitVitalAttributesEffectClass);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());

	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayEffect>> InitAttributesEffectClasses;
		
		if (InitVitalAttributesEffectClass) InitAttributesEffectClasses.Add(InitVitalAttributesEffectClass);
		if (InitPrimaryAttributesEffectClass) InitAttributesEffectClasses.Add(InitPrimaryAttributesEffectClass);
		
		AbilitySystemComponent->Init(this, this, InitAttributesEffectClasses, TArray<TSubclassOf<UGameplayAbility>> {});
	}

	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		if (const auto ItemClass = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemClass)
		{
			Equipment.SetEquipmentSlotActor(EquipmentSlot, AttachItem(ItemClass));
		}
		
		if (Equipment.GetEquipmentSlotData(EquipmentSlot).bActivated) ActivateEquipment(EquipmentSlot);
	}
}

void AUpCharacter::Die()
{
	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		if (const auto ItemActor = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor)
		{
			ItemActor->Detach();
			Equipment.SetEquipmentSlotActor(EquipmentSlot, nullptr);
		}
	}

	if (const auto Mesh = GetMesh())
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		Mesh->SetEnableGravity(true);
		Mesh->SetSimulatePhysics(true);
	}

	if (const auto CapsuleComponent = GetCapsuleComponent())
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	return TArray<UUpAttributeSet*> { VitalAttributeSet, PrimaryAttributeSet };
}

bool AUpCharacter::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	DeactivateEquipment(EquipmentSlot);

	if (const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);
		EquipmentSlotData.ItemInstance.ItemClass && !EquipmentSlotData.bActivated)
	{
		if (const auto DefaultObject = EquipmentSlotData.ItemInstance.ItemClass.GetDefaultObject())
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
			{
				if (const auto& ItemData = GameInstance->GetItemData(DefaultObject->GetTagId()); ItemData.IsValid())
				{
					if (ItemData.ResultingPosture != EUpCharacterPosture::Casual)
					{
						Posture = ItemData.ResultingPosture;
					}

					if (EquipmentSlotData.ItemInstance.ItemActor)
					{
						ShowAttachedItem(EquipmentSlotData.ItemInstance.ItemActor,
							ItemData.SocketTag.IsValid() ? ItemData.SocketTag.GetTagName() : NAME_None);
					}

					if (AbilitySystemComponent)
					{
						for (const auto& AbilityGrantSpec : ItemData.AbilityGrantSpecs)
						{
							if (!AbilityGrantSpec.IsValid()) continue;
					
							AbilitySystemComponent->GrantAbility(AbilityGrantSpec.AbilityClass);
						}
					}
			
					Equipment.ActivateEquipmentSlot(EquipmentSlot);
					OnEquipmentActivation(EquipmentSlot);

					return true;
				}
			}
		}
	}

	return false;
}

bool AUpCharacter::DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);
	
	if (!EquipmentSlotData.bActivated) return true;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
		GameInstance && EquipmentSlotData.ItemInstance.ItemClass)
	{
		if (const auto DefaultObject = EquipmentSlotData.ItemInstance.ItemClass.GetDefaultObject())
		{
			if (const auto& ItemData = GameInstance->GetItemData(DefaultObject->GetTagId()); ItemData.IsValid())
			{
				if (ItemData.ResultingPosture != EUpCharacterPosture::Casual)
				{
					Posture = EUpCharacterPosture::Casual;
				}

				if (EquipmentSlotData.ItemInstance.ItemActor) HideAttachedItem(EquipmentSlotData.ItemInstance.ItemActor);

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
			
				Equipment.DeactivateEquipmentSlot(EquipmentSlot);
				OnEquipmentDeactivation(EquipmentSlot);
			
				return true;
			}
		}
	}

	return false;
}

AUpItem* AUpCharacter::AttachItem(const TSubclassOf<AUpItem> ItemClass)
{
	if (const auto World = GetWorld())
	{
		const auto SpawnLocation = GetActorLocation();
		const auto SpawnRotation= GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		if (const auto ItemActor = Cast<AUpItem>(World->SpawnActor(ItemClass, &SpawnLocation, &SpawnRotation, SpawnParams)))
		{
			HideAttachedItem(ItemActor);
			
			return ItemActor;
		}
	}

	return nullptr;
}

void AUpCharacter::HideAttachedItem(AUpItem* ItemActor) const
{
	if (const auto Mesh = GetMesh())
	{
		ItemActor->AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		ItemActor->SetActorEnableCollision(false);
		ItemActor->SetActorHiddenInGame(true);
	}
}

void AUpCharacter::ShowAttachedItem(AUpItem* ItemActor, const FName& SocketName) const
{
	if (const auto Mesh = GetMesh())
	{
		ItemActor->AttachToComponentWithScaling(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), SocketName);
		ItemActor->SetActorHiddenInGame(false);
	}
}
