// Copyright AB. All Rights Reserved.

#include "Characters/UpNpcCharacter.h"

#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Components/UpCombatComponent.h"
#include "Components/UpDialogueComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "GAS/Attributes/UpVitalAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpNpcCharacter::AUpNpcCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	AIControllerClass = AUpAiController::StaticClass();

	if (const auto CharacterMovement = GetCharacterMovement())
	{
		CharacterMovement->bUseControllerDesiredRotation = true;
	}
	
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CombatComponent = CreateDefaultSubobject<UUpCombatComponent>(TEXT("CombatComponent"));
	DialogueComponent = CreateDefaultSubobject<UUpDialogueComponent>(TEXT("DialogueComponent"));

	VitalAttributeSet = CreateDefaultSubobject<UUpVitalAttributeSet>(TEXT("VitalAttributeSet"));
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

void AUpNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		NpcData = GameInstance->GetNpcData(TagId);
	}
	
	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayEffect>> InitAttributesEffectClasses;
		
		if (InitVitalAttributesEffectClass) InitAttributesEffectClasses.Add(InitVitalAttributesEffectClass);
		if (InitPrimaryAttributesEffectClass) InitAttributesEffectClasses.Add(InitPrimaryAttributesEffectClass);
		
		AbilitySystemComponent->Init(this, this, InitAttributesEffectClasses, TArray<TSubclassOf<UGameplayAbility>> {});
	}
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

FUpCharacterEquipment AUpNpcCharacter::GetCharacterEquipment() const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		return GameInstance->GetNpcEquipment(TagId);
	}

	return FUpCharacterEquipment();
}

void AUpNpcCharacter::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot, const FUpEquipmentSlotData& EquipmentSlotData)
{
	DeactivateEquipment(EquipmentSlot);
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->ActivateNpcEquipmentSlot(TagId, EquipmentSlot);
		
		if (const auto& ItemData = GameInstance->GetItemData(EquipmentSlotData.ItemInstance.ItemTagId); ItemData.IsValid())
		{
			if (ItemData.ResultingPosture != EUpCharacterPosture::Casual)
			{
				Posture = ItemData.ResultingPosture;
			}

			if (ItemData.StaticMeshActorClass)
			{
				if (const auto World = GetWorld())
				{
					if (const auto Mesh = GetMesh())
					{
						const auto SpawnLocation = GetActorLocation();
						const auto SpawnRotation= GetActorRotation();
				
						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = this;

						if (ItemData.ItemCategory == EUpItemCategory::Weapon)
						{
							WeaponActor = Cast<AStaticMeshActor>(World->SpawnActor(ItemData.StaticMeshActorClass, &SpawnLocation, &SpawnRotation, SpawnParams));
					
							if (IsValid(WeaponActor))
							{
								WeaponActor->SetActorEnableCollision(false);
								WeaponActor->AttachToComponent(Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false),
									ItemData.SocketTag.IsValid() ? ItemData.SocketTag.GetTagName() : NAME_None);
							}
						}
					}
				}
			}
		}
	}
}

void AUpNpcCharacter::DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto& EquipmentSlotData = GameInstance->GetNpcEquipment(TagId).GetEquipmentSlotData(EquipmentSlot);
			EquipmentSlotData.IsValid() && EquipmentSlotData.bActivated)
		{
			GameInstance->DeactivateNpcEquipmentSlot(TagId, EquipmentSlot);
			
			if (const auto& ItemData = GameInstance->GetItemData(EquipmentSlotData.ItemInstance.ItemTagId); ItemData.IsValid())
			{
				if (ItemData.ResultingPosture != EUpCharacterPosture::Casual)
				{
					Posture = EUpCharacterPosture::Casual;
				}

				if (ItemData.ItemCategory == EUpItemCategory::Weapon && IsValid(WeaponActor))
				{
					WeaponActor->Destroy();
					WeaponActor = nullptr;
				}
			}
		}
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

void AUpNpcCharacter::GrantTagSpec(const FUpTagSpec& TagSpec)
{
	UUpBlueprintFunctionLibrary::GrantNpcTagSpec(this, TagId, TagSpec);
}

void AUpNpcCharacter::JumpToLocation(const FVector& TargetLocation, const float Duration)
{
	const auto ActorLocation = GetActorLocation();

	SetYaw(UKismetMathLibrary::FindLookAtRotation(ActorLocation, TargetLocation).Yaw);

	if (CustomMovementComponent)
	{
		LaunchCharacter(UUpBlueprintFunctionLibrary::CalculateVelocity(
			ActorLocation, TargetLocation, Duration, CustomMovementComponent->GravityScale), true, true);
	}
}

bool AUpNpcCharacter::Mantle() const
{
	if (!CustomMovementComponent) return false;
	
	return CustomMovementComponent->TryMantle();
}

void AUpNpcCharacter::ToggleSprint(const bool bSprint) const
{
	if (!CustomMovementComponent) return;

	CustomMovementComponent->ToggleSprint(bSprint);
}
