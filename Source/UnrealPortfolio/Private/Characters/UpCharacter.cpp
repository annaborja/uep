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
	
	check(InitPrimaryAttributesEffectClass);
	check(InitVitalAttributesEffectClass);

	check(ClimbingMontage_ThirdPerson);
	check(GesturesMontage_ThirdPerson);
	check(HitReactionsMontage_ThirdPerson);
	check(MantlesMontage_ThirdPerson);
	check(ReloadsMontage_ThirdPerson);
	
	check(Sfx_JumpLaunches);

	CustomMovementComponent = CastChecked<UUpCharacterMovementComponent>(GetCharacterMovement());

	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;
		GetAbilityClassesToGrant(AbilityClasses);
		
		AbilitySystemComponent->Init(this, this,
			TArray { InitVitalAttributesEffectClass, InitPrimaryAttributesEffectClass }, AbilityClasses);
		AbilitySystemComponent->RegisterGameplayTagEvent(TAG_Cooldown_StaminaRegen, EGameplayTagEventType::AnyCountChange)
			.AddUObject(this, &ThisClass::HandleAbilitySystemTagEvent);
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

bool AUpCharacter::IsBusy() const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);

		if (AbilitySystemTags.HasTagExact(TAG_State_Busy))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s is busy"), *GetName())
			return true;
		}

		return false;
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

		if (IsInFirstPersonMode()) ItemActor->ToggleCastShadows(false);
	}
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
				TraceEnd += -FVector(0.0, 0.0, 1.0) * TraceLength;
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
