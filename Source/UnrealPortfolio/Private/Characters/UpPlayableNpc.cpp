// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableNpc.h"

#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpDialogueComponent.h"
#include "GAS/Attributes/UpHealthAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpPlayableNpc::AUpPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	AIControllerClass = AUpAiController::StaticClass();

	if (const auto CharacterMovement = GetCharacterMovement())
	{
		CharacterMovement->bUseControllerDesiredRotation = true;
	}
	
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
		NpcData = GameInstance->GetNpcData(TagId);
	}
}

void AUpPlayableNpc::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	AiController = Cast<AUpAiController>(NewController);

	if (AiController && BehaviorTree)
	{
		if (const auto BlackboardComponent = AiController->GetBlackboardComponent())
		{
			if (const auto BlackboardAsset = BehaviorTree->BlackboardAsset)
			{
				BlackboardComponent->InitializeBlackboard(*BlackboardAsset);
				AiController->RunBehaviorTree(BehaviorTree);
			}
		}
	}
}

void AUpPlayableNpc::OnEquipmentActivation(const FUpItemData& ItemData)
{
	Super::OnEquipmentActivation(ItemData);

	if (ItemData.ItemCategory == EUpItemCategory::Weapon)
	{
		if (CustomPlayerController)
		{
			if (const auto GunInputMappingContext = CustomPlayerController->GetGunInputMappingContext())
			{
				CustomPlayerController->ActivateInputMappingContext(GunInputMappingContext, false, 1);
			}
		}
	}
}

void AUpPlayableNpc::OnEquipmentDeactivation(const FUpItemData& ItemData)
{
	Super::OnEquipmentDeactivation(ItemData);

	if (ItemData.ItemCategory == EUpItemCategory::Weapon)
	{
		if (CustomPlayerController)
		{
			if (const auto GunInputMappingContext = CustomPlayerController->GetGunInputMappingContext())
			{
				CustomPlayerController->DeactivateInputMappingContext(GunInputMappingContext);
			}
		}
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
			
bool AUpPlayableNpc::CanInteract() const
{
	return DialogueComponent->HasAvailableDialogue();
}

void AUpPlayableNpc::Interact(AUpPlayerController* PlayerController)
{
	if (DialogueComponent->HasAvailableDialogue())
	{
		DialogueComponent->StartDialogue(PlayerController);
	}
}

void AUpPlayableNpc::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	UUpBlueprintFunctionLibrary::GrantNpcTagSpec(this, TagId, TagSpec);
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