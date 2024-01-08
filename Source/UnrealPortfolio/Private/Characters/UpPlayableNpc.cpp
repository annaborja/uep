// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableNpc.h"

#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpDialogueComponent.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpPlayableNpc::AUpPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	DialogueComponent = CreateDefaultSubobject<UUpDialogueComponent>(TEXT("DialogueComponent"));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);
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

void AUpPlayableNpc::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->GetNpcCharacterTags(TagId, TagContainer);
	}

	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);
		TagContainer.AppendTags(AbilitySystemTags);
	}
}

FUpInteractionData AUpPlayableNpc::GetInteractionData(const AUpPlayerController* PlayerController)
{
	if (DialogueComponent->HasAvailableDialogue())
	{
		return FUpInteractionData(this, FText::FromString(FString::Printf(TEXT("Talk to %s"), *GetInGameName().ToString())));
	}

	return FUpInteractionData();
}

void AUpPlayableNpc::Interact(AUpPlayerController* PlayerController)
{
	if (DialogueComponent->HasAvailableDialogue())
	{
		DialogueComponent->StartDialogue(PlayerController);
	}
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
	
	return CustomMovementComponent->TryMantle();
}

void AUpPlayableNpc::ToggleSprint(const bool bSprint) const
{
	if (!CustomMovementComponent) return;

	CustomMovementComponent->ToggleSprint(bSprint);
}
