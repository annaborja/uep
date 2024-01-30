// Copyright AB. All Rights Reserved.

#include "Characters/UpCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
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
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Tags/AttributeTags.h"
#include "Tags/GasTags.h"
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

	if (const auto MovementComponent = GetCharacterMovement())
	{
		MovementComponent->GroundFriction = 2.f;
		MovementComponent->BrakingDecelerationWalking = 85.f;
	}
	
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CombatComponent = CreateDefaultSubobject<UUpCombatComponent>(TEXT("CombatComponent"));
	
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));
	VitalAttributeSet = CreateDefaultSubobject<UUpVitalAttributeSet>(TEXT("VitalAttributeSet"));
}

void AUpCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(EffectClass_InitPrimaryAttributes);
	check(EffectClass_InitVitalAttributes);

	check(ClimbingMontage_ThirdPerson);
	check(DeathsMontage_ThirdPerson);
	check(GesturesMontage_ThirdPerson);
	check(GunFiringMontage_ThirdPerson);
	check(HitReactionsMontage_ThirdPerson);
	check(MantlesMontage_ThirdPerson);
	check(ReloadsMontage_ThirdPerson);
	check(WeaponEquipMontage_ThirdPerson);

	check(Sfx_BulletImpacts);
	check(Sfx_JumpLaunches);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());

	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;
		AbilityClasses.Append(AbilityClassesToGrant);
		GetAbilityClassesToGrant(AbilityClasses);
		
		AbilitySystemComponent->Init(this, this,
			TArray { EffectClass_InitVitalAttributes, EffectClass_InitPrimaryAttributes }, AbilityClasses);
		AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Cooldown_StaminaRegen, EGameplayTagEventType::AnyCountChange)
			.AddUObject(this, &ThisClass::HandleAbilitySystemTagEvent);
	}

	for (const auto EquipmentSlot : FUpCharacterEquipment::GetWeaponSlots())
	{
		if (const auto ItemClass = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemClass)
		{
			Equipment.SetEquipmentSlotActor(EquipmentSlot, SpawnAndAttachItem(ItemClass));
		}
		
		if (Equipment.GetEquipmentSlotData(EquipmentSlot).bActivated)
		{
			Equipment.DeactivateEquipmentSlot(EquipmentSlot);
			
			FGameplayEventData EventPayload;
			EventPayload.EventMagnitude = EquipmentSlot;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_Ability_ActivateEquipment, EventPayload);
		}
	}
}

float AUpCharacter::CalculateDamageDealt(const FHitResult& HitResult) const
{
	return 20.f;
}

void AUpCharacter::Die()
{
	if (const auto CapsuleComponent = GetCapsuleComponent())
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (const auto Mesh = GetMesh())
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetEnableGravity(true);
		Mesh->SetSimulatePhysics(true);
	}

	if (Controller) Controller->UnPossess();
}

float AUpCharacter::GetHorizontalSpeed() const
{
	return UKismetMathLibrary::VSizeXY(GetVelocity());
}

float AUpCharacter::GetMovementOffsetYaw() const
{
	return UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(GetVelocity()), GetBaseAimRotation()).Yaw;
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

bool AUpCharacter::IsAiming() const
{
	if (AbilitySystemComponent) return AbilitySystemComponent->HasMatchingGameplayTag(TAG_State_Aiming);

	return false;
}

bool AUpCharacter::IsBusy() const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);

		if (AbilitySystemTags.HasTagExact(TAG_State_Busy))
		{
			// UE_LOG(LogTemp, Warning, TEXT("%s is busy"), *GetName())
			return true;
		}

		return false;
	}

	return false;
}

bool AUpCharacter::IsDead() const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);

		return AbilitySystemTags.HasTagExact(TAG_State_Dead);
	}

	return false;
}

bool AUpCharacter::IsInFirstPersonMode() const
{
	const auto CameraView = GetCameraView();

	return CameraView == EUpCameraView::FirstPerson || CameraView == EUpCameraView::FirstPerson_Debug;
}

bool AUpCharacter::IsInStrafingMode() const
{
	const auto CameraView = GetCameraView();
	
	return IsInFirstPersonMode() || CameraView == EUpCameraView::ThirdPerson_OverTheShoulder || CameraView == EUpCameraView::ThirdPerson_OverTheShoulder_Debug;
}

bool AUpCharacter::IsShooting() const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);

		return AbilitySystemTags.HasTagExact(TAG_State_Shooting);
	}

	return false;
}

bool AUpCharacter::IsStrafingForward() const
{
	return IsInStrafingMode() && FMath::Abs(GetMovementOffsetYaw()) < 90.f;
}

void AUpCharacter::SetRelaxed(const bool bInRelaxed)
{
	bRelaxed = bInRelaxed;

	if (const auto& WeaponSlotData = Equipment.GetPotentialActiveWeaponSlotData(); WeaponSlotData.bActivated)
	{
		if (const auto Weapon = Cast<AUpWeapon>(WeaponSlotData.ItemInstance.ItemActor); Weapon && Weapon->IsRifleType())
		{
			if (const auto Mesh = GetMesh())
			{
				auto SocketNameString = FString::Printf(TEXT("%s.%s"), *Weapon->GetWeaponTypeNameSectionString(), NAME_STRING_ACTIVATED);

				if (bRelaxed)
				{
					SocketNameString += TEXT(".");
					SocketNameString += NAME_STRING_RELAXED;
				}
				
				Weapon->AttachToComponentWithScaling(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(SocketNameString));
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
	AttachDeactivatedItem(ItemActor);

	OnItemEquip(ItemActor, EquipmentSlot);

	if (bSlotActivated) ActivateEquipment(EquipmentSlot);
}

void AUpCharacter::UnequipItem(const EUpEquipmentSlot::Type EquipmentSlot)
{
	if (const auto ItemActor = Equipment.GetEquipmentSlotData(EquipmentSlot).ItemInstance.ItemActor)
	{
		ItemActor->Detach();
		Equipment.DeactivateEquipmentSlot(EquipmentSlot);
		Equipment.SetEquipmentSlotActor(EquipmentSlot, nullptr);
		Equipment.SetEquipmentSlotClass(EquipmentSlot, nullptr);

		OnItemUnequip(EquipmentSlot);
	}
}

bool AUpCharacter::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);

	if (EquipmentSlotData.bActivated) return true;
	
	if (const auto Item = EquipmentSlotData.ItemInstance.ItemActor)
	{
		// If we're activating a weapon slot, make sure all other weapon slots are deactivated.
		if (FUpCharacterEquipment::IsWeaponSlot(EquipmentSlot))
		{
			for (const auto WeaponSlot : FUpCharacterEquipment::GetWeaponSlots())
			{
				if (WeaponSlot != EquipmentSlot) DeactivateEquipment(WeaponSlot);
			}
		}
	
		if (const auto ResultingPosture = Item->GetCharacterPosture(); ResultingPosture != EUpCharacterPosture::Casual)
		{
			Posture = ResultingPosture;
		}

		AttachActivatedItem(Item);

		if (AbilitySystemComponent)
		{
			for (const auto& AbilityGrantSpec : Item->GetItemData().AbilityGrantSpecs)
			{
				if (!AbilityGrantSpec.IsValid()) continue;
	
				AbilitySystemComponent->GrantAbility(AbilityGrantSpec.AbilityClass);
			}
		}

		Equipment.ActivateEquipmentSlot(EquipmentSlot);
		OnEquipmentActivation(EquipmentSlot);

		return true;
	}

	return false;
}

bool AUpCharacter::DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot);
	
	if (!EquipmentSlotData.bActivated) return true;
	
	if (const auto Item = EquipmentSlotData.ItemInstance.ItemActor)
	{
		if (const auto ResultingPosture = Item->GetCharacterPosture(); ResultingPosture != EUpCharacterPosture::Casual)
		{
			Posture = EUpCharacterPosture::Casual;
		}

		AttachDeactivatedItem(Item);

		if (AbilitySystemComponent)
		{
			for (const auto& AbilityGrantSpec : Item->GetItemData().AbilityGrantSpecs)
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

	return false;
}

AUpWeapon* AUpCharacter::GetActiveWeapon() const
{
	if (const auto& EquipmentSlotData = GetEquipment().GetPotentialActiveWeaponSlotData(); EquipmentSlotData.bActivated)
	{
		return Cast<AUpWeapon>(EquipmentSlotData.ItemInstance.ItemActor);
	}

	return nullptr;
}

void AUpCharacter::AttachActivatedItem(AUpItem* ItemActor)
{
	if (const auto Mesh = GetMesh())
	{
		if (!ItemActor->IsAttachedTo(this)) ItemActor->Attach(this);

		FString NameString = TEXT("");

		if (const auto Weapon = Cast<AUpWeapon>(ItemActor))
		{
			NameString += Weapon->GetWeaponTypeNameSectionString();
		}

		NameString += TEXT(".");
		NameString += NAME_STRING_ACTIVATED;

		if (NameString.StartsWith(NAME_STRING_RIFLE_TYPE) && bRelaxed)
		{
			NameString += TEXT(".");
			NameString += NAME_STRING_RELAXED;
		}
		
		ItemActor->AttachToComponentWithScaling(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(NameString));
	}
}

void AUpCharacter::AttachDeactivatedItem(AUpItem* ItemActor)
{
	if (const auto Mesh = GetMesh())
	{
		if (!ItemActor->IsAttachedTo(this)) ItemActor->Attach(this);

		FString NameString = TEXT("");

		if (const auto Weapon = Cast<AUpWeapon>(ItemActor))
		{
			NameString += Weapon->GetWeaponTypeNameSectionString();
		}

		NameString += TEXT(".");
		NameString += NAME_STRING_DEACTIVATED;
		
		ItemActor->AttachToComponentWithScaling(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(NameString));
	}
}

void AUpCharacter::HandleFootstep(const FName& BoneName, const EUpTraceDirection::Type TraceDirection, const float TraceLength, const float VolumeMultiplier) const
{
	HandleNoise(SfxMap_Footsteps, BoneName, TraceDirection, TraceLength);
}

void AUpCharacter::HandleJumpLaunch() const
{
	if (!Sfx_JumpLaunches) return;

	const auto PlayableCharacter = Cast<AUpPlayableCharacter>(this);
	
	UGameplayStatics::PlaySoundAtLocation(this, Sfx_JumpLaunches, GetActorLocation(), GetActorRotation(),
		PlayableCharacter && PlayableCharacter->IsPlayer() ? 1.f : 0.5f);
}

void AUpCharacter::HandleLanding(const FName& BoneName, const EUpTraceDirection::Type TraceDirection, const float TraceLength, const float VolumeMultiplier) const
{
	HandleNoise(SfxMap_JumpLandings, BoneName, TraceDirection, TraceLength, VolumeMultiplier);
}

void AUpCharacter::HandleAbilitySystemTagEvent(const FGameplayTag Tag, const int32 Count)
{
	if (bDebugGas)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAS tag event: %s (%d)"), *Tag.GetTagName().ToString(), Count)
	}

	if (Tag.MatchesTagExact(TAG_Cooldown_StaminaRegen) && Count <= 0 && AbilitySystemComponent)
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(TAG_Ability_StaminaRegen);
					
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
	}
}

void AUpCharacter::GetAbilityClassesToGrant(TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses) const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
		{
			AbilityClasses.Append(GasDataAsset->GetGrantedAbilityClasses_Character());
		}
	}
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
			AttachDeactivatedItem(ItemActor);
			
			return ItemActor;
		}
	}

	return nullptr;
}

void AUpCharacter::HandleNoise(const TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> SfxMap, const FName& BoneName,
	const EUpTraceDirection::Type TraceDirection, const float TraceLength, const float VolumeMultiplier) const
{
	if (const auto World = GetWorld())
	{
		if (const auto Mesh = GetMesh())
		{
			FCollisionQueryParams QueryParams;
			QueryParams.bReturnPhysicalMaterial = true;

			const auto TraceStart = BoneName.IsNone() ? GetActorLocation() : Mesh->GetBoneLocation(BoneName);
			auto TraceEnd = TraceStart;

			if (TraceDirection == EUpTraceDirection::Forward)
			{
				TraceEnd += GetActorForwardVector() * TraceLength;
			} else
			{
				TraceEnd += FVector::DownVector * TraceLength;
			}
			
			FHitResult HitResult;
			World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

			if (bDebugPhysicalMaterials)
			{
				UKismetSystemLibrary::DrawDebugLine(this, TraceStart, TraceEnd, FColor::Cyan, 5.f);
			}

			if (const auto HitActor = HitResult.GetActor())
			{
				const auto SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
				const auto SoundPtr = SfxMap.Find(SurfaceType);

				if (bDebugPhysicalMaterials)
				{
					UE_LOG(LogTemp, Warning, TEXT("Landing surface type: %d (actor %s, blocking hit: %d)"),
						SurfaceType, *HitActor->GetName(), HitResult.bBlockingHit)
				}
				
				if (const auto Sound = SoundPtr == nullptr ? nullptr : *SoundPtr)
				{
					const auto PlayableCharacter = Cast<AUpPlayableCharacter>(this);
					
					UGameplayStatics::PlaySoundAtLocation(this, Sound, HitResult.ImpactPoint, GetActorRotation(),
						(PlayableCharacter && PlayableCharacter->IsPlayer() ? 1.f : 0.5f) * VolumeMultiplier);
				} else
				{
					UE_LOG(LogTemp, Error, TEXT("No sound for surface type %d for hit actor %s"), SurfaceType, *HitActor->GetName())
				}
			}
		}
	}
}

void AUpCharacter::OnItemEquip(AUpItem* ItemActor, const EUpEquipmentSlot::Type EquipmentSlot)
{
	if (const auto Sound = ItemActor->GetSfx_Equip())
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
}
