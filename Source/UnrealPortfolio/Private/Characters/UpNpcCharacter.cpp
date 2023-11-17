// Copyright AB. All Rights Reserved.

#include "Characters/UpNpcCharacter.h"

#include "Characters/Player/UpPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpDialogueComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealPortfolio/UnrealPortfolioGameModeBase.h"
#include "Utils/Constants.h"

bool AUpNpcCharacter::GrantTagSpec(AUnrealPortfolioGameModeBase* GameMode, const FGameplayTag& NpcTagId, const FUpTagSpec& TagSpec)
{
	if (GameMode->ShouldDebugTagSpecGrant())
    {
    	UE_LOG(LogTemp, Warning, TEXT("%s GrantTagSpec: %s (%d)"), *NpcTagId.ToString(), *TagSpec.Tag.ToString(), TagSpec.Count)
    }

    if (TagSpec.IsValid())
    {
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

    return false;
}

AUpNpcCharacter::AUpNpcCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
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

	check(DialogueVoice);
	check(TagId.IsValid());

	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (const auto NpcDataTable = GameMode->GetNpcDataTable())
		{
			TArray<FUpNpcData*> AllRows;
			NpcDataTable->GetAllRows<FUpNpcData>(TEXT("NpcDataTable GetAllRows"), AllRows);

			for (const auto Row : AllRows)
			{
				if (Row->TagId.MatchesTagExact(TagId))
				{
					NpcData = *Row;
					break;
				}
			}
		}
	}
	
	if (AbilitySystemComponent) AbilitySystemComponent->Init(this, this);
}

void AUpNpcCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->GetNpcCharacterTags(TagId, TagContainer);
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
	if (const auto GameMode = Cast<AUnrealPortfolioGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GrantTagSpec(GameMode, TagId, TagSpec);
	}
}
