// Copyright AB. All Rights Reserved.

#include "Characters/UpPlayableNpc.h"

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
#include "GAS/Abilities/UpGameplayAbility.h"
#include "GAS/Attributes/UpHealthAttributeSet.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
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
	
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CombatComponent = CreateDefaultSubobject<UUpCombatComponent>(TEXT("CombatComponent"));
	DialogueComponent = CreateDefaultSubobject<UUpDialogueComponent>(TEXT("DialogueComponent"));

	HealthAttributeSet = CreateDefaultSubobject<UUpHealthAttributeSet>(TEXT("HealthAttributeSet"));
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));

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
	
	if (AbilitySystemComponent)
	{
		TArray<TSubclassOf<UGameplayEffect>> InitAttributesEffectClasses;
		
		if (InitHealthAttributesEffectClass) InitAttributesEffectClasses.Add(InitHealthAttributesEffectClass);
		if (InitPrimaryAttributesEffectClass) InitAttributesEffectClasses.Add(InitPrimaryAttributesEffectClass);
		
		AbilitySystemComponent->Init(this, this, InitAttributesEffectClasses, TArray<TSubclassOf<UGameplayAbility>> {});
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

FUpCharacterEquipment AUpPlayableNpc::GetCharacterEquipment() const
{
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		return GameInstance->GetNpcEquipment(TagId);
	}

	return FUpCharacterEquipment();
}

void AUpPlayableNpc::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot, const FUpEquipmentSlotData& EquipmentSlotData)
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

			if (AbilitySystemComponent)
			{
				for (const auto& AbilityGrantSpec : ItemData.AbilityGrantSpecs)
				{
					if (!AbilityGrantSpec.IsValid()) continue;
					
					AbilitySystemComponent->GrantAbility(AbilityGrantSpec.AbilityClass);
				}
			}

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
	}
}

void AUpPlayableNpc::DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
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

				if (ItemData.ItemCategory == EUpItemCategory::Weapon)
				{
					if (IsValid(WeaponActor))
					{
						WeaponActor->Destroy();
						WeaponActor = nullptr;
					}

					if (CustomPlayerController)
					{
						if (const auto GunInputMappingContext = CustomPlayerController->GetGunInputMappingContext())
						{
							CustomPlayerController->DeactivateInputMappingContext(GunInputMappingContext);
						}
					}
				}
			}
		}
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
