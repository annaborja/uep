// Copyright AB. All Rights Reserved.

#include "Components/UpCharacterMovementComponent.h"

#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/UpGasDataAsset.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "Items/UpItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpCharacterMovementComponent::UUpCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;

	GravityScale = 2.f;
	JumpZVelocity = 640.f;
	MaxWalkSpeed = 400.f;
	MaxWalkSpeedCrouched = 135.f;
	bUseAccelerationForPaths = true;
	bUseControllerDesiredRotation = true;
}

void UUpCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<AUpCharacter>(GetOwner());
	PlayableCharacter = Cast<AUpPlayableCharacter>(GetOwner());

	BaseGravityScale = GravityScale;
	BaseMaxWalkSpeed = MaxWalkSpeed;
	BaseMaxSprintSpeed = MaxSprintSpeed;
	BaseRotationRate = RotationRate;
	
	OnAnimMontageEndedDelegate.BindUFunction(this, FName("HandleAnimMontageEnded"));
	OnPlayerJumpApexReachedDelegate.BindUFunction(this, FName("OnPlayerJumpApexReached"));
}

float UUpCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking && !IsCrouching() && bWantsToSprint) return MaxSprintSpeed;
	
	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();
	
	switch(CustomMovementMode)
	{
	case EUpCustomMovementMode::None:
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid custom movement mode %d"), CustomMovementMode)
		return 0.f;
	}
}

void UUpCharacterMovementComponent::OnMovementModeChanged(const EMovementMode PreviousMovementMode, const uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (bIsPlayer && PlayableCharacter)
	{
		if (PreviousMovementMode == MOVE_Falling)
		{
			GravityScale = BaseGravityScale;
		}

		// We use the flying movement mode for special jump mechanics like mantling.
		if (PreviousMovementMode == MOVE_Falling || PreviousMovementMode == MOVE_Flying)
		{
			if (!PlayableCharacter->IsAllowedToJump())
			{
				PlayableCharacter->GetWorldTimerManager().SetTimer(JumpCooldownTimerHandle, this, &ThisClass::AllowJump, JumpCooldownTime);
			}
		}
	}
}

void UUpCharacterMovementComponent::UpdateCharacterStateBeforeMovement(const float DeltaSeconds)
{
	if (Character)
	{
		if (const auto GameMode = UUpBlueprintFunctionLibrary::GetGameMode(this))
		{
			if (const auto AttributeSet = Character->GetPrimaryAttributeSet())
			{
				if (const auto SpeedAttribute = AttributeSet->GetSpeedAttribute(); SpeedAttribute.IsValid())
				{
					const auto SpeedModifier = SpeedAttribute.GetNumericValue(AttributeSet) - 50.f;
					const auto SpeedModifierMultiplier = GameMode->GetWalkSpeedModifierMultiplier();
			
					MaxWalkSpeed = BaseMaxWalkSpeed + SpeedModifier * SpeedModifierMultiplier;
					MaxSprintSpeed = BaseMaxSprintSpeed + SpeedModifier * SpeedModifierMultiplier;
				}
			}
		}

		if (bTransitionRootMotionSourceFinished)
		{
			if (PostTransitionMontageData.IsValid())
			{
				// Need to be in flying mode for vertical root motion to work.
				SetMovementMode(MOVE_Flying);

				CharacterOwner->PlayAnimMontage(PostTransitionMontageData.Montage, PostTransitionMontageData.Rate, PostTransitionMontageData.StartSection);

				// HACK: We reuse third-person Mixamo animations for our first-person skeleton,
				// and the translation process doesn't properly bring over root motion.
				if (Character->IsInFirstPersonCameraView())
				{
					MainRootMotionSource.Reset();
					MainRootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
			
					MainRootMotionSource->AccumulateMode = ERootMotionAccumulateMode::Override;
					MainRootMotionSource->Duration = PostTransitionMontageData.Montage->GetSectionLength(
						PostTransitionMontageData.Montage->GetSectionIndex(PostTransitionMontageData.StartSection));
					MainRootMotionSource->StartLocation = MainRootMotionStartLocation;
					MainRootMotionSource->TargetLocation = MainRootMotionEndLocation;

					// Root motion affects velocity (not position), so zero out the velocity before applying root motion.
					Velocity = FVector::ZeroVector;
					MainRootMotionSourceId = ApplyRootMotionSource(MainRootMotionSource);
				}
				
				PostTransitionMontageData = FUpMontageData();
			}
			else
			{
				SetMovementMode(MOVE_Walking);
			}

			bTransitionRootMotionSourceFinished = false;
		}

		if (const auto RootMotionSource = GetRootMotionSourceByID(MainRootMotionSourceId);
			RootMotionSource && RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::Prepared))
		{
			if (Character->GetActorEnableCollision()) Character->SetActorEnableCollision(false);

			if (PlayableCharacter)
			{
				if  (const auto SpringArm = PlayableCharacter->GetSpringArmComponent(); SpringArm && SpringArm->bDoCollisionTest)
				{
					SpringArm->bDoCollisionTest = false;
				}
			}
		}
	}

	if (bIsPlayer && PlayableCharacter)
	{
		// If the player is trying to jump...
		if (bCustomPressedJump)
		{
			// Try mantling.
			if (TryMantle())
			{
				PlayableCharacter->StopJumping();
			} else
			{
				// If mantling didn't work, revert to UE's default jump behavior.
				bCustomPressedJump = false;

				PlayableCharacter->bPressedJump = true;
				PlayableCharacter->CheckJumpInput(DeltaSeconds);
			}
		} else if (MovementMode == MOVE_Falling)
		{
			TryMantle();
		}
	}
	
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UUpCharacterMovementComponent::UpdateCharacterStateAfterMovement(const float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

	// Switch to the correct two-handed gun socket when necessary (alert vs relaxed).
	if (Character && !Character->IsRelaxed())
	{
		if (const auto& WeaponSlotData = Character->GetCharacterEquipment().GetPotentialActiveWeaponSlotData(); WeaponSlotData.bActivated)
		{
			if (const auto WeaponActor = WeaponSlotData.ItemInstance.ItemActor;
				WeaponActor && UUpBlueprintFunctionLibrary::IsTwoHandedGunTag(WeaponActor->GetTagId()))
			{
				if (const auto Mesh = Character->GetMesh())
				{
					const auto ParentSocketName = WeaponActor->GetAttachParentSocketName();

					if (UKismetMathLibrary::VSizeXY(Velocity) > MaxWalkSpeed + 5.f)
					{
						if (const auto DesiredSocketName = TAG_Socket_TwoHandedGun_Relaxed.GetTag().GetTagName();
							!ParentSocketName.IsEqual(DesiredSocketName))
						{
							WeaponActor->AttachToComponentWithScaling(Mesh,
								FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), DesiredSocketName);
						}
					} else
					{
						if (const auto DesiredSocketName = TAG_Socket_TwoHandedGun.GetTag().GetTagName();
							!ParentSocketName.IsEqual(DesiredSocketName))
						{
							WeaponActor->AttachToComponentWithScaling(Mesh,
								FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), DesiredSocketName);
						}
					}
				}
			}
		}
	}

	// Check whether a main root motion source has just finished.
	if (const auto RootMotionSource = GetRootMotionSourceByID(MainRootMotionSourceId);
		RootMotionSource && RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::Finished) && Character)
	{
		Character->SetActorEnableCollision(true);

		if (PlayableCharacter)
		{
			if  (const auto SpringArm = PlayableCharacter->GetSpringArmComponent())
			{
				SpringArm->bDoCollisionTest = true;
			}
		}
		
		RemoveRootMotionSourceByID(MainRootMotionSourceId);
		bMainRootMotionSourceFinished = true;
	}

	// Reset the movement mode if a root motion animation (e.g., mantling) has just finished.
	if (((!HasAnimRootMotion() && bHadAnimRootMotion) || bMainRootMotionSourceFinished) && MovementMode == MOVE_Flying)
	{
		bMainRootMotionSourceFinished = false;
		
		SetMovementMode(MOVE_Walking);

		if (Character && Character->HasRootMotionTargetLocation())
		{
			if (const auto RootMotionTargetLocation = Character->GetRootMotionTargetLocation(); RootMotionTargetLocation != Character->GetActorLocation())
			{
				Character->LaunchCharacter(RootMotionTargetLocation - Character->GetActorLocation(), true, false);
			}

			Character->UnsetRootMotionTargetLocation();
		}
	}

	// Check whether a transition root motion source has just finished.
	if (const auto RootMotionSource = GetRootMotionSourceByID(TransitionRootMotionSourceId);
		RootMotionSource && RootMotionSource->Status.HasFlag(ERootMotionSourceStatusFlags::Finished))
	{
		RemoveRootMotionSourceByID(TransitionRootMotionSourceId);
		bTransitionRootMotionSourceFinished = true;
	}
	
	bHadAnimRootMotion = HasAnimRootMotion();
}

void UUpCharacterMovementComponent::InitForPlayer()
{
	bIsPlayer = true;
	bOrientRotationToMovement = true;
	bUseControllerDesiredRotation = false;
    
	if (Character && !Character->OnReachedJumpApex.Contains(OnPlayerJumpApexReachedDelegate))
	{
		Character->OnReachedJumpApex.Add(OnPlayerJumpApexReachedDelegate);
	}
}

void UUpCharacterMovementComponent::TearDownForPlayer()
{
	bIsPlayer = false;
	bOrientRotationToMovement = false;
	bUseControllerDesiredRotation = true;
    	
	if (Character && Character->OnReachedJumpApex.Contains(OnPlayerJumpApexReachedDelegate))
	{
		Character->OnReachedJumpApex.Remove(OnPlayerJumpApexReachedDelegate);
	}
}

bool UUpCharacterMovementComponent::TryMantle()
{
	// Can only mantle if we're walking/standing or jumping/falling.
	if (!(MovementMode == MOVE_Walking && !IsCrouching()) && MovementMode != MOVE_Falling) return false;
	
	if (!Character) return false;

	if (const auto World = GetWorld())
	{
		if (const auto Capsule = Character->GetCapsuleComponent())
		{
			const auto CapsuleHalfHeight = UKismetMathLibrary::Conv_FloatToDouble(Capsule->GetScaledCapsuleHalfHeight());
			const auto CapsuleRadius = UKismetMathLibrary::Conv_FloatToDouble(Capsule->GetScaledCapsuleRadius());
		
			const auto CapsuleBaseLocation = UpdatedComponent->GetComponentLocation() + FVector::DownVector * CapsuleHalfHeight;
			const auto ForwardNormal2d = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
		
			// The valid forward distance of the mantle is based on the character's velocity.
			const auto ValidForwardDistance = FMath::Clamp(UKismetMathLibrary::VSizeXY(Velocity) * 0.25f,
				CapsuleRadius + MantleMinDistanceOffset, UKismetMathLibrary::Conv_FloatToDouble(MantleMaxDistance));
		
			// Enable mantling starting from the height at which the character can't just step up onto something.
			const auto MinMantleHeight = MaxStepHeight - 1.f;
			const auto MaxMantleHeight = CapsuleHalfHeight * 2.0 + MantleReachHeight;
		
			auto FrontTraceStart = CapsuleBaseLocation + FVector::UpVector * UKismetMathLibrary::Conv_FloatToDouble(MinMantleHeight);
			FHitResult FrontSurfaceHit;
	
			for (uint8 i = 0; i < NumMantleFrontTraceDivisions + 1; i++)
			{
				if (UKismetSystemLibrary::LineTraceSingle(this,
					FrontTraceStart, FrontTraceStart + ForwardNormal2d * ValidForwardDistance,
					UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_CLIMBABLE), false, TArray<AActor*> { Character },
					bDebugMantle ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, FrontSurfaceHit, true))
				{
					break;
				}
		
				FrontTraceStart += FVector::UpVector * ((MaxMantleHeight - MinMantleHeight) / NumMantleFrontTraceDivisions);
			}
	
			// No valid front surface to mantle.
			if (!FrontSurfaceHit.IsValidBlockingHit()) return false;
	
			const auto WallSteepnessAngle = FMath::RadiansToDegrees(FMath::Acos(FrontSurfaceHit.ImpactNormal.Dot(FVector::UpVector)));
			const auto WallApproachAngle = FMath::RadiansToDegrees(FMath::Acos(ForwardNormal2d.Dot(-FrontSurfaceHit.ImpactNormal)));
	
			if (bDebugMantle)
			{
				UE_LOG(LogTemp, Warning, TEXT("TryMantle WallSteepnessAngle = %g, WallApproachAngle = %g"), WallSteepnessAngle, WallApproachAngle)
			}
	
			// Wall or approach angle is invalid.
			if (WallSteepnessAngle < MantleMinWallSteepnessAngle || WallApproachAngle > MantleMaxAlignmentAngle) return false;
	
			const auto WallUpVector = FVector::VectorPlaneProject(FVector::UpVector, FrontSurfaceHit.ImpactNormal).GetSafeNormal();
			FHitResult TopSurfaceHit;

			// Offset the top trace slightly inward to make sure it overlaps the mantle-able object (versus skimming the surface).
			if (const auto TopTraceEnd = FrontSurfaceHit.ImpactPoint + ForwardNormal2d;
				!UKismetSystemLibrary::LineTraceSingle(this, TopTraceEnd + WallUpVector *
						(MaxMantleHeight - (FrontSurfaceHit.ImpactPoint.Z - CapsuleBaseLocation.Z)) / WallUpVector.Dot(FVector::UpVector), TopTraceEnd,
					UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_CLIMBABLE), false, TArray<AActor*> { Character },
					bDebugMantle ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, TopSurfaceHit, true))
			{
				return false;
			}

			// No valid top surface to mantle.
			if (!TopSurfaceHit.IsValidBlockingHit()) return false;
	
			const auto SurfaceCos = TopSurfaceHit.ImpactNormal.Dot(FVector::UpVector);
			const auto MantleSurfaceAngle = FMath::RadiansToDegrees(FMath::Acos(SurfaceCos));
			const auto SurfaceHeight = (TopSurfaceHit.Location - CapsuleBaseLocation).Dot(FVector::UpVector);
	
			if (bDebugMantle)
			{
				UE_LOG(LogTemp, Warning, TEXT("TryMantle MantleSurfaceAngle = %g, SurfaceHeight = %g"), MantleSurfaceAngle, SurfaceHeight)
			}
		
			if (MantleSurfaceAngle > MantleMaxSurfaceAngle || SurfaceHeight > MaxMantleHeight) return false;

			const auto SlopeWidth = CapsuleRadius;
			const auto SlopeLength = SlopeWidth / FMath::Cos(FMath::DegreesToRadians(180 - 90 - (90 - MantleSurfaceAngle)));
			const auto SlopeHeight = FMath::Sqrt(FMath::Pow(SlopeLength, 2) - FMath::Pow(SlopeWidth, 2));
			const auto ImpactNormalCos = ForwardNormal2d.Dot(TopSurfaceHit.ImpactNormal);

			if (bDebugMantle)
			{
				UE_LOG(LogTemp, Warning, TEXT("TryMantle SlopeHeight = %g, ImpactNormalCos = %g"), SlopeHeight, ImpactNormalCos)
			}
	
			MainRootMotionEndLocation = TopSurfaceHit.Location + ForwardNormal2d * CapsuleRadius +
				FVector::UpVector * (CapsuleHalfHeight + 1.0 + (ImpactNormalCos > 0 ? -1 : 1) * SlopeHeight);

			const auto CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(Character);
		
			if (World->OverlapBlockingTestByProfile(MainRootMotionEndLocation, FQuat::Identity, TEXT("BlockAll"),
				CollisionShape, CollisionQueryParams))
			{
				if (bDebugMantle)
				{
					UKismetSystemLibrary::DrawDebugCapsule(this, MainRootMotionEndLocation,
						CapsuleHalfHeight, CapsuleRadius, Character->GetActorRotation(), FColor::Red, 4.f);
				}
			
				return false;
			}

			if (bDebugMantle)
			{
				UKismetSystemLibrary::DrawDebugCapsule(this, MainRootMotionEndLocation,
					CapsuleHalfHeight, CapsuleRadius, Character->GetActorRotation(), FColor::Green, 4.f);
			}

			const auto TallMantleStartLocation = GetMantleStartLocation(FrontSurfaceHit, TopSurfaceHit,
				CapsuleHalfHeight, CapsuleRadius, MinMantleHeight, true);
			const auto VerticalSpeed = Velocity.Dot(FVector::UpVector);
			
			// Execute a "tall" mantle if:
			//   - the top mantle surface is higher than the character's head, OR
			//   - the character is falling with downward velocity
			const auto bTallMantle = (MovementMode == MOVE_Walking && SurfaceHeight > CapsuleHalfHeight * 2.0) ||
				(MovementMode == MOVE_Falling && VerticalSpeed < 0.0 &&
					!World->OverlapBlockingTestByProfile(TallMantleStartLocation, FQuat::Identity, TEXT("BlockAll"),
						CollisionShape, CollisionQueryParams));

			const auto RootMotionTransitionStartLocation = UpdatedComponent->GetComponentLocation();
			
			MainRootMotionStartLocation = bTallMantle ? TallMantleStartLocation :
				GetMantleStartLocation(FrontSurfaceHit, TopSurfaceHit, CapsuleHalfHeight, CapsuleRadius, MinMantleHeight, false);
			
			if (bDebugMantle)
			{
				UKismetSystemLibrary::DrawDebugCapsule(this, MainRootMotionStartLocation,
					CapsuleHalfHeight, CapsuleRadius, Character->GetActorRotation(), FColor::Yellow, 4.f);
			}
		
			if (const auto MantlesMontage = Character->GetMantlesMontage())
			{
				TransitionRootMotionSource.Reset();
				TransitionRootMotionSource = MakeShared<FRootMotionSource_MoveToForce>();
			
				TransitionRootMotionSource->AccumulateMode = ERootMotionAccumulateMode::Override;
				TransitionRootMotionSource->Duration = FMath::Clamp(
					FVector::Dist(RootMotionTransitionStartLocation, MainRootMotionStartLocation) / 500.0, 0.1, 0.25);
				TransitionRootMotionSource->StartLocation = RootMotionTransitionStartLocation;
				TransitionRootMotionSource->TargetLocation = MainRootMotionStartLocation;

				// Root motion affects velocity (not position), so zero out the velocity before applying root motion.
				Velocity = FVector::ZeroVector;
				// Need to be in flying mode for vertical root motion to work.
				SetMovementMode(MOVE_Flying);
				TransitionRootMotionSourceId = ApplyRootMotionSource(TransitionRootMotionSource);

				PostTransitionMontageData = FUpMontageData(MantlesMontage, FMath::GetMappedRangeValueClamped(
					FVector2D(-500.0, 750.0), FVector2D(0.9, 1.2), VerticalSpeed));
				PostTransitionMontageData.StartSection = bTallMantle ? TEXT("MantleTall") : TEXT("MantleShort");

				MantlesMontageEndCount = 0;
				
				Character->PlayAnimMontage(MantlesMontage, 1 / TransitionRootMotionSource->Duration,
					bTallMantle ? TEXT("MantleTallTransition") : TEXT("MantleShortTransition"));

				if (const auto Mesh = Character->GetMesh())
				{
					if (const auto AnimInstance = Mesh->GetAnimInstance())
					{
						if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(World))
						{
							if (const auto GasDataAsset = GameInstance->GetGasDataAsset())
							{
								if (const auto BusyEffectClass = GasDataAsset->GetBusyEffectClass())
								{
									if (const auto AbilitySystemComponent = Character->GetAbilitySystemComponent())
									{
										BusyEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
											*AbilitySystemComponent->MakeOutgoingSpec(BusyEffectClass, 1.f, AbilitySystemComponent->MakeEffectContext()).Data.Get());

										if (!AnimInstance->OnMontageEnded.Contains(OnAnimMontageEndedDelegate))
										{
											AnimInstance->OnMontageEnded.Add(OnAnimMontageEndedDelegate);
										}
									}
								}
							}
						}
					}
				}
			}
			
			return true;
		}
	}

	return false;
}

void UUpCharacterMovementComponent::AllowJump()
{
	if (!PlayableCharacter) return;

	PlayableCharacter->AllowJump();
}

void UUpCharacterMovementComponent::HandleAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!Character) return;

	if (Character->GetMantlesMontage() == Montage) MantlesMontageEndCount++;

	if (MantlesMontageEndCount >= 2)
	{
		if (const auto AbilitySystemComponent = Character->GetAbilitySystemComponent())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(BusyEffectHandle);
		}
	}
}

bool UUpCharacterMovementComponent::IsCustomMovementModeActive( const EUpCustomMovementMode::Type InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

// TODO(P1): Add comments for this function (see explanation at https://youtu.be/3hk39el8dg8?feature=shared&t=4562).
FVector UUpCharacterMovementComponent::GetMantleStartLocation(const FHitResult& FrontSurfaceHit, const FHitResult& TopSurfaceHit,
	const float CapsuleHalfHeight, const float CapsuleRadius, const float MinMantleHeight, const bool bTallMantle) const
{
	const auto DownDistance = bTallMantle ? CapsuleHalfHeight * 2.f : MinMantleHeight;
	const auto EdgeTangent = FVector::CrossProduct(TopSurfaceHit.Normal, FrontSurfaceHit.Normal).GetSafeNormal();
	
	auto MantleStartLocation = TopSurfaceHit.Location;
	MantleStartLocation += FrontSurfaceHit.Normal.GetSafeNormal2D() * (2.f + CapsuleRadius);
	MantleStartLocation += UpdatedComponent->GetForwardVector().GetSafeNormal2D().ProjectOnTo(EdgeTangent) * CapsuleRadius * 0.3f;
	MantleStartLocation += FVector::UpVector * CapsuleHalfHeight;
	MantleStartLocation += FVector::DownVector * DownDistance;
	MantleStartLocation += FrontSurfaceHit.Normal.GetSafeNormal2D() * FrontSurfaceHit.Normal.Dot(FVector::UpVector) * DownDistance;

	return MantleStartLocation;
}
