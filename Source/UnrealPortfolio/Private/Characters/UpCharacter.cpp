// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "GameplayEffect.h"
#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "Characters/UpPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpCombatComponent.h"
#include "GAS/UpGasDataAsset.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Items/UpWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpCharacter::AUpCharacter()
{
	AIControllerClass = AUpAiController::StaticClass();
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
	
	check(InitPrimaryAttributesEffectClass);
	check(InitVitalAttributesEffectClass);
	
	check(GesturesMontage_ThirdPerson);
	check(HitReactionsMontage_ThirdPerson);
	check(MantlesMontage_ThirdPerson);
	check(ReloadsMontage_ThirdPerson);
	
	check(Sfx_JumpLaunches);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());

	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;

		if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
		{
			if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
			{
				AbilityClasses.Append(GasDataAsset->GetGrantedAbilityClasses_Character());
			}
		}
		
		AbilitySystemComponent->Init(this, this,
			TArray { InitVitalAttributesEffectClass, InitPrimaryAttributesEffectClass }, AbilityClasses);
	}

	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		if (const auto ItemClass = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemClass)
		{
			Equipment.SetEquipmentSlotActor(EquipmentSlot, SpawnAndAttachItem(ItemClass));
		}
		
		if (Equipment.GetEquipmentSlotData(EquipmentSlot).bActivated) ActivateEquipment(EquipmentSlot);
	}
}

void AUpCharacter::Die()
{
	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		UnequipItem(EquipmentSlot);
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

FRotator AUpCharacter::GetSafeRotation() const
{
	// In case we're in first-person view, remove any non-yaw rotation.
	auto Result = GetActorRotation();
	Result.Pitch = 0.0;
	Result.Roll = 0.0;

	return Result;
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

bool AUpCharacter::IsInFirstPersonCameraView() const
{
	const auto CameraView = GetCameraView();

	return CameraView == EUpCameraView::FirstPerson || CameraView == EUpCameraView::FirstPerson_Debug;
}

void AUpCharacter::SetRelaxed(const bool bInRelaxed)
{
	bRelaxed = bInRelaxed;

	if (const auto& WeaponSlotData = Equipment.GetPotentialActiveWeaponSlotData(); WeaponSlotData.bActivated)
	{
		if (const auto WeaponActor = WeaponSlotData.ItemInstance.ItemActor;
			WeaponActor && UUpBlueprintFunctionLibrary::IsTwoHandedGunTag(WeaponActor->GetTagId()))
		{
			if (const auto Mesh = GetMesh())
			{
				WeaponActor->AttachToComponentWithScaling(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false),
					bRelaxed ? TAG_Socket_TwoHandedGun_Relaxed.GetTag().GetTagName() : TAG_Socket_TwoHandedGun.GetTag().GetTagName());
			}
		}
	}
}

TArray<UUpAttributeSet*> AUpCharacter::GetAttributeSets() const
{
	return TArray<UUpAttributeSet*> { VitalAttributeSet, PrimaryAttributeSet };
}

void AUpCharacter::EquipItem(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot)
{
	const auto bSlotActivated = Equipment.GetEquipmentSlotData(EquipmentSlot).bActivated;
	
	UnequipItem(EquipmentSlot);
	
	Equipment.SetEquipmentSlotClass(EquipmentSlot, ItemActor->GetClass());
	Equipment.SetEquipmentSlotActor(EquipmentSlot, ItemActor);
	AttachAndHideItem(ItemActor);

	OnItemEquip(ItemActor, EquipmentSlot);

	if (bSlotActivated) ActivateEquipment(EquipmentSlot);
}

void AUpCharacter::UnequipItem(const EUpEquipmentSlot::Type EquipmentSlot)
{
	if (const auto ItemActor = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor)
	{
		ItemActor->Detach();
		Equipment.SetEquipmentSlotActor(EquipmentSlot, nullptr);
		Equipment.SetEquipmentSlotClass(EquipmentSlot, nullptr);
		Equipment.DeactivateEquipmentSlot(EquipmentSlot);
		
		OnItemUnequip(EquipmentSlot);
	}
}

bool AUpCharacter::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	DeactivateEquipment(EquipmentSlot);
	
	if (const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);
		EquipmentSlotData.ItemInstance.ItemClass && !EquipmentSlotData.bActivated)
	{
		// If we're activating a weapon slot, make sure all other weapon slots are deactivated.
		if (FUpCharacterEquipment::IsWeaponSlot(EquipmentSlot))
		{
			for (const auto WeaponSlot : FUpCharacterEquipment::GetWeaponSlots())
			{
				if (WeaponSlot != EquipmentSlot) DeactivateEquipment(WeaponSlot);
			}
		}
		
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
						auto SocketTag = ItemData.SocketTag.IsValid() ? ItemData.SocketTag : TAG_Socket_None;

						if (SocketTag.MatchesTagExact(TAG_Socket_TwoHandedGun) && bRelaxed)
						{
							SocketTag = TAG_Socket_TwoHandedGun_Relaxed;
						}
						
						AttachAndShowItem(EquipmentSlotData.ItemInstance.ItemActor,
							SocketTag.MatchesTagExact(TAG_Socket_None) ? NAME_None : SocketTag.GetTagName());
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

				if (EquipmentSlotData.ItemInstance.ItemActor) AttachAndHideItem(EquipmentSlotData.ItemInstance.ItemActor);

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

void AUpCharacter::HandleFootstep() const
{
	HandleLanding(SfxMap_Footsteps);
}

void AUpCharacter::HandleJumpLaunch() const
{
	if (!Sfx_JumpLaunches) return;

	const auto PlayableCharacter = Cast<AUpPlayableCharacter>(this);
	
	UGameplayStatics::PlaySoundAtLocation(this, Sfx_JumpLaunches, GetActorLocation(), GetActorRotation(),
		PlayableCharacter && PlayableCharacter->IsPlayer() ? 1.f : 0.5f);
}

void AUpCharacter::HandleJumpLand() const
{
	HandleLanding(SfxMap_JumpLandings);
}

AUpItem* AUpCharacter::SpawnAndAttachItem(const TSubclassOf<AUpItem> ItemClass)
{
	if (const auto World = GetWorld())
	{
		const auto SpawnLocation = GetActorLocation();
		const auto SpawnRotation= GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		if (const auto ItemActor = Cast<AUpItem>(World->SpawnActor(ItemClass, &SpawnLocation, &SpawnRotation, SpawnParams)))
		{
			AttachAndHideItem(ItemActor);
			
			return ItemActor;
		}
	}

	return nullptr;
}

void AUpCharacter::AttachAndHideItem(AUpItem* ItemActor)
{
	if (const auto Mesh = GetMesh())
	{
		if (!ItemActor->IsAttachedTo(this)) ItemActor->Attach(this);
		
		ItemActor->AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		ItemActor->SetActorHiddenInGame(true);
	}
}

void AUpCharacter::AttachAndShowItem(AUpItem* ItemActor, const FName& SocketName) const
{
	if (const auto Mesh = GetMesh())
	{
		ItemActor->AttachToComponentWithScaling(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), SocketName);
		ItemActor->SetActorHiddenInGame(false);

		if (IsInFirstPersonCameraView()) ItemActor->ToggleCastShadows(false);
	}
}

void AUpCharacter::HandleLanding(const TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap) const
{
	if (const auto World = GetWorld())
	{
		if (const auto Capsule = GetCapsuleComponent())
		{
			const auto TraceStart = GetActorLocation();

			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial =  true;
			
			FHitResult HitResult;
			World->LineTraceSingleByChannel(HitResult, TraceStart,
				TraceStart + GetActorUpVector() * -1.0 * (Capsule->GetScaledCapsuleHalfHeight() + 10.0),
				ECC_Visibility, QueryParams);

			if (HitResult.bBlockingHit)
			{
				const auto SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
				const auto SoundPtr = SfxMap.Find(SurfaceType);
				
				if (const auto Sound = SoundPtr == nullptr ? nullptr : *SoundPtr)
				{
					const auto PlayableCharacter = Cast<AUpPlayableCharacter>(this);
					
					UGameplayStatics::PlaySoundAtLocation(this, Sound, HitResult.ImpactPoint, GetActorRotation(),
						PlayableCharacter && PlayableCharacter->IsPlayer() ? 1.f : 0.5f);
				} else
				{
					if (const auto HitActor = HitResult.GetActor())
					{
						UE_LOG(LogTemp, Error, TEXT("No sound for surface type %d for hit actor %s"), SurfaceType, *HitActor->GetName())
					}
				}
			}
		}
	}
}
