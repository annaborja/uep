// Copyright AB. All Rights Reserved.

#include "Characters/UpNonPlayableNpc.h"

#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/UpCombatComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GAS/UpAbilitySystemComponent.h"
#include "GAS/Attributes/UpHealthAttributeSet.h"
#include "GAS/Attributes/UpPrimaryAttributeSet.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpNonPlayableNpc::AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	AIControllerClass = AUpAiController::StaticClass();

	if (const auto CharacterMovement = GetCharacterMovement())
	{
		CharacterMovement->bUseControllerDesiredRotation = true;
	}
	
	AbilitySystemComponent = CreateDefaultSubobject<UUpAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CombatComponent = CreateDefaultSubobject<UUpCombatComponent>(TEXT("CombatComponent"));

	HealthAttributeSet = CreateDefaultSubobject<UUpHealthAttributeSet>(TEXT("HealthAttributeSet"));
	PrimaryAttributeSet = CreateDefaultSubobject<UUpPrimaryAttributeSet>(TEXT("PrimaryAttributeSet"));
}

void AUpNonPlayableNpc::PossessedBy(AController* NewController)
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

void AUpNonPlayableNpc::ActivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot, const FUpEquipmentSlotData& EquipmentSlotData)
{
	DeactivateEquipment(EquipmentSlot);
	Equipment.ActivateEquipmentSlot(EquipmentSlot);
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
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
		}
	}
}

void AUpNonPlayableNpc::DeactivateEquipment(const EUpEquipmentSlot::Type EquipmentSlot)
{
	Equipment.DeactivateEquipmentSlot(EquipmentSlot);
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto& EquipmentSlotData = Equipment.GetEquipmentSlotData(EquipmentSlot); EquipmentSlotData.IsValid() && EquipmentSlotData.bActivated)
		{
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
				}
			}
		}
	}
}

void AUpNonPlayableNpc::BeginPlay()
{
	Super::BeginPlay();
}
