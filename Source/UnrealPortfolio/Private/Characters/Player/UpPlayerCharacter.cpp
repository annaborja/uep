// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCharacter.h"

#include "UpGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/Components/UpPlayerCombatComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpPlayerCharacter::AUpPlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);

	PlayerCombatComponent = CreateDefaultSubobject<UUpPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerInteractionComponent = CreateDefaultSubobject<UUpPlayerInteractionComponent>(TEXT("PlayerInteractionComponent"));
}

void AUpPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		FGameplayTagContainer PartyMemberTags;
		GameInstance->GetPartyMemberTags(PartyMemberTags);

		const auto PartyMemberTag = PartyMemberTags.First();
		
		TArray<AActor*> PlayableNpcs;
		UGameplayStatics::GetAllActorsOfClass(this, AUpPlayableNpc::StaticClass(), PlayableNpcs);
		AUpPlayableNpc* PartyMember = nullptr;

		for (const auto Actor : PlayableNpcs)
		{
			if (const auto TagIdable = Cast<IUpTagIdable>(Actor); TagIdable && TagIdable->GetTagId().MatchesTagExact(PartyMemberTag))
			{
				PartyMember = Cast<AUpPlayableNpc>(Actor);
				break;
			}
		}

		if (CustomPlayerController && PartyMember) CustomPlayerController->SwitchCharacter(PartyMember);
	}
}

void AUpPlayerCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->GetPlayerCharacterTags(TagContainer);
	}

	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilitySystemTags;
		AbilitySystemComponent->GetOwnedGameplayTags(AbilitySystemTags);
		TagContainer.AppendTags(AbilitySystemTags);
	}
}
