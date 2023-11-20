// Copyright AB. All Rights Reserved.

#include "Characters/UpNpcCharacter.h"

#include "AI/UpAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpDialogueComponent.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

bool AUpNpcCharacter::GrantTagSpec(AUnrealPortfolioGameModeBase* GameMode, const FGameplayTag& NpcTagId, const FUpTagSpec& TagSpec)
{
	if (GameMode->ShouldDebugTagSpecGrant())
    {
    	UE_LOG(LogTemp, Warning, TEXT("%s GrantTagSpec: %s (%d)"), *NpcTagId.ToString(), *TagSpec.Tag.ToString(), TagSpec.Count)
    }

    bool bSuccess = false;

    if (TagSpec.Count > 0)
    {
    	bSuccess = GameMode->AddNpcCharacterTag(NpcTagId, TagSpec.Tag);
    } else if (TagSpec.Count < 0)
    {
    	bSuccess = GameMode->RemoveNpcCharacterTag(NpcTagId, TagSpec.Tag);
    }

    return bSuccess;
}

AUpNpcCharacter::AUpNpcCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	AIControllerClass = AUpAiController::StaticClass();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	if (const auto CharacterMovement = GetCharacterMovement())
	{
		CharacterMovement->bUseControllerDesiredRotation = true;
	}
	
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

	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	DialogueComponent = CreateDefaultSubobject<UUpDialogueComponent>(TEXT("DialogueComponent"));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

void AUpNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GameMode = UUpBlueprintFunctionLibrary::GetGameMode<AUnrealPortfolioGameModeBase>(this))
	{
		if (const auto NpcDataTable = GameMode->GetNpcDataTable())
		{
			TArray<FUpNpcData*> AllNpcDataRows;
			NpcDataTable->GetAllRows<FUpNpcData>(TEXT("NpcDataTable GetAllRows"), AllNpcDataRows);

			for (const auto NpcDataRow : AllNpcDataRows)
			{
				if (NpcDataRow->TagId.MatchesTagExact(TagId))
				{
					NpcData = *NpcDataRow;
					break;
				}
			}
		}
	}
	
	if (AbilitySystemComponent) AbilitySystemComponent->Init(this, this);
}

void AUpNpcCharacter::PossessedBy(AController* NewController)
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

void AUpNpcCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameMode = UUpBlueprintFunctionLibrary::GetGameMode<AUnrealPortfolioGameModeBase>(this))
	{
		GameMode->GetNpcCharacterTags(TagId, TagContainer);
	}

	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);
		TagContainer.AppendTags(AbilitySystemTags);
	}
}

bool AUpNpcCharacter::CanInteract() const
{
	return DialogueComponent->HasAvailableDialogue();
}

void AUpNpcCharacter::Interact(AUpPlayerController* PlayerController)
{
	if (DialogueComponent->HasAvailableDialogue())
	{
		DialogueComponent->StartDialogue(PlayerController);
	}
}

FText AUpNpcCharacter::GetInGameName() const
{
	return NpcData.Name;
}

void AUpNpcCharacter::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	if (const auto GameMode = UUpBlueprintFunctionLibrary::GetGameMode<AUnrealPortfolioGameModeBase>(this))
	{
		GrantTagSpec(GameMode, TagId, TagSpec);
	}
}

void AUpNpcCharacter::ToggleSprint(const bool bSprint) const
{
	if (!CustomMovementComponent) return;

	CustomMovementComponent->ToggleSprint(bSprint);
}
