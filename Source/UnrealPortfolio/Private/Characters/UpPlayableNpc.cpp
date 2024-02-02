// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableNpc.h"

#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpDialogueComponent.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpPlayableNpc::AUpPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	DialogueComponent = CreateDefaultSubobject<UUpDialogueComponent>(TEXT("DialogueComponent"));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());
	InteractionSphere->SetSphereRadius(100.f);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

void AUpPlayableNpc::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		CharacterData = GameInstance->GetCharacterData(TagId);
	}
}

void AUpPlayableNpc::Die()
{
	Super::Die();

	if (InteractionSphere) UUpBlueprintFunctionLibrary::TearDownActorComponent(InteractionSphere);
}

void AUpPlayableNpc::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	Super::GetOwnedGameplayTags(TagContainer);
	
	FGameplayTagContainer NpcTags;
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->GetNpcCharacterTags(TagId, NpcTags);
	}

	TagContainer.AppendTags(NpcTags);
}

FUpInteractionData AUpPlayableNpc::GetInteractionData(const AController* Controller)
{
	if (DialogueComponent->HasAvailableDialogue())
	{
		return FUpInteractionData(this, FText::FromString(FString::Printf(TEXT("Talk to %s"), *GetInGameName().ToString())));
	}

	return FUpInteractionData();
}

bool AUpPlayableNpc::Interact(AController* Controller)
{
	if (const auto CustomPlayerController = Cast<AUpPlayerController>(Controller))
	{
		if (DialogueComponent->HasAvailableDialogue())
		{
			DialogueComponent->StartDialogue(CustomPlayerController);
			return false;
		}
	}

	return true;
}

void AUpPlayableNpc::JumpToLocation(const FVector& TargetLocation, const float Duration)
{
	const auto ActorLocation = GetActorLocation();

	SetYaw(UKismetMathLibrary::FindLookAtRotation(ActorLocation, TargetLocation).Yaw);

	if (CustomMovementComponent)
	{
		LaunchCharacter(UUpBlueprintFunctionLibrary::CalculateVelocity(
			ActorLocation, TargetLocation, Duration, CustomMovementComponent->GravityScale), true, true);
	}
}

bool AUpPlayableNpc::Mantle() const
{
	if (!CustomMovementComponent) return false;

	const auto TraceStart = GetActorLocation();
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceStart + GetActorForwardVector() * 100.f,
		UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_CLIMBABLE), false, TArray<AActor*> {},
		CustomMovementComponent->ShouldDebugMantle() ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, OutHit, true);
	
	return CustomMovementComponent->TryMantle(OutHit.bBlockingHit ? (OutHit.ImpactPoint - TraceStart).Size2D() : -1.f);
}
