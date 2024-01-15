// Copyright AB. All Rights Reserved.

#include "Characters/Player/UpPlayerCharacter.h"

#include "UpGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/UpPlayerState.h"
#include "Characters/Player/Components/UpPlayerCombatComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Attributes/UpAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpHud.h"
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
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (CustomPlayerController)
	{
		if (const auto CustomHud = CustomPlayerController->GetCustomHud())
		{
			if (const auto CustomPlayerState = GetPlayerState<AUpPlayerState>())
			{
				FGameplayTagContainer SquadMemberTags;
				CustomPlayerState->GetSquadMemberTags(SquadMemberTags);
		
				const auto PlayerLocation = GetActorLocation();
				const auto PlayerForwardVector= GetActorForwardVector();
				const auto PlayerRightVector= GetActorRightVector();
				const auto PlayerRotation = GetActorRotation();
		
				AUpPlayableNpc* SquadMember1 = nullptr;

				if (const auto World = GetWorld())
				{
					uint8 SquadMemberNumber = 0;
			
					for (const auto& Tag : SquadMemberTags)
					{
						for (const auto NpcClass : PlayableNpcClasses)
						{
							if (const auto DefaultObject = NpcClass->GetDefaultObject<AUpPlayableNpc>();
								DefaultObject && DefaultObject->GetTagId().MatchesTagExact(Tag))
							{
								SquadMemberNumber++;
								auto NpcLocation = PlayerLocation;

								if (SquadMemberNumber == 2)
								{
									NpcLocation += PlayerForwardVector * SquadMemberSpawnLocationOffset_Forward +
										PlayerRightVector * SquadMemberSpawnLocationOffset_Right * -1.0;
								} else if (SquadMemberNumber > 2)
								{
									NpcLocation += PlayerForwardVector * SquadMemberSpawnLocationOffset_Forward +
										PlayerRightVector * SquadMemberSpawnLocationOffset_Right;
								}

								FActorSpawnParameters SpawnParams;
								SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
						
								const auto Npc = Cast<AUpPlayableNpc>(World->SpawnActor(NpcClass, &NpcLocation, &PlayerRotation, SpawnParams));

								if (SquadMemberNumber == 1)
								{
									SquadMember1 = Npc;
								} else
								{
									if (Npc) CustomHud->BroadcastSecondarySquadMember(Npc);
								}

								if (Npc)
								{
									if (const auto NpcAbilitySystemComponent = Npc->GetAbilitySystemComponent())
									{
										for (const auto AttributeSet : Npc->GetAttributeSets())
										{
											for (const auto TagAttributeMapping : AttributeSet->GetTagAttributeMap())
											{
												NpcAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TagAttributeMapping.Value())
													.AddLambda([this, AttributeSet, CustomHud, Npc, TagAttributeMapping](const FOnAttributeChangeData& Data)
													{
														if (IsValid(CustomHud))
														{
															CustomHud->BroadcastAttributeValue(Npc->GetTagId(), TagAttributeMapping.Key, TagAttributeMapping.Value(), AttributeSet);
														}
													});
											}
										}	
									}
								}
							}
						}			
					}
				}

				if (CustomPlayerController && SquadMember1) CustomPlayerController->SwitchCharacter(SquadMember1);
			}
		}
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
