// Copyright AB. All Rights Reserved.

#include "Items/UpItem.h"

#include "UpGameInstance.h"
#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Components/SphereComponent.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpItem::AUpItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());
	InteractionSphere->SetSphereRadius(50.f);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

FUpInteractionData AUpItem::GetInteractionData(const AController* Controller)
{
	if (!CanInteract(Controller)) return FUpInteractionData();
	
	return FUpInteractionData(this,
		GetInteractionPromptText(Controller), GetInteractionPromptSubText(Controller));
}

bool AUpItem::Interact(AController* Controller)
{
	if (const auto CustomPlayerController = Cast<AUpPlayerController>(Controller))
	{
		if (const auto PossessedCharacter = CustomPlayerController->GetPossessedCharacter())
		{
			const auto& DynamicRelatedTag = GetInteractionRelatedTag(Controller);
		
			FUpTagSpec TagSpec(TagId, GetInteractionQuantity(Controller, DynamicRelatedTag));
			TagSpec.RelatedCount = GetInteractionRelatedQuantity(Controller, DynamicRelatedTag);
		
			if (DynamicRelatedTag.IsValid())
			{
				TagSpec.RelatedTag = DynamicRelatedTag;
			}
		
			if (PossessedCharacter->GrantTagSpec(TagSpec))
			{
				if (bDestroyOnInteract) Destroy();
			}
		}
	}

	return true;
}

void AUpItem::AttachToComponentWithScaling(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, const FName& SocketName)
{
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attach %s to %s at socket %s"), *GetName(), *Parent->GetName(), *SocketName.ToString())
	}
	
	AttachToComponent(Parent, AttachmentRules, SocketName);

	if (const auto ItemClass = GetClass())
	{
		if (const auto DefaultObject = Cast<AUpItem>(ItemClass->GetDefaultObject()))
		{
			SetActorScale3D(DefaultObject->GetActorRelativeScale3D());
		}
	}
}

void AUpItem::Attach(AActor* Actor)
{
	AttachToActor(Actor, FAttachmentTransformRules::SnapToTargetIncludingScale);
	SetActorEnableCollision(false);

	const auto RootStaticMeshComponent = GetStaticMeshComponent();

	TArray<UActorComponent*> Components;
	GetComponents(UStaticMeshComponent::StaticClass(), Components);

	for (const auto Component : Components)
	{
		if (const auto StaticMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			if (Component == RootStaticMeshComponent)
			{
				RootStaticMeshComponent->SetEnableGravity(false);
				RootStaticMeshComponent->SetSimulatePhysics(false);
			}
		}
	}
}

void AUpItem::Detach()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);

	const auto RootMesh = GetStaticMeshComponent();

	TArray<UActorComponent*> Components;
	GetComponents(UStaticMeshComponent::StaticClass(), Components);

	for (const auto Component : Components)
	{
		if (const auto ComponentMesh = Cast<UStaticMeshComponent>(Component))
		{
			ComponentMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			ComponentMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			ComponentMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			
			if (Component == RootMesh)
			{
				RootMesh->SetEnableGravity(true);
				RootMesh->SetSimulatePhysics(true);
			}
		}
	}

	if (InteractionSphere) InteractionSphere->SetUsingAbsoluteScale(true);
}

void AUpItem::ToggleCastShadows(const bool bEnable) const
{
	TArray<UActorComponent*> Components;
	GetComponents(UStaticMeshComponent::StaticClass(), Components);

	for (const auto Component : Components)
	{
		if (const auto StaticMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			StaticMeshComponent->SetCastShadow(bEnable);
		}
	}
}

void AUpItem::BeginPlay()
{
	Super::BeginPlay();

	check(TagId.IsValid());

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		ItemData = GameInstance->GetItemData(ItemDataTagId.IsValid() ? ItemDataTagId : TagId);
	}

	check(ItemData.IsValid())
}
