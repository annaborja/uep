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
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

FUpInteractionData AUpItem::GetInteractionData()
{
	return FUpInteractionData(this, GetInteractionPrompt());
}

void AUpItem::Interact(AUpPlayerController* PlayerController)
{
	if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
	{
		const auto& DynamicRelatedTag = GetInteractionRelatedTag(PlayerController);
		
		FUpTagSpec TagSpec(TagId, GetInteractionQuantity(PlayerController, DynamicRelatedTag));
		
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

void AUpItem::AttachToComponentWithScaling(USceneComponent* Parent, const FAttachmentTransformRules& AttachmentRules, const FName& SocketName)
{
	AttachToComponent(Parent, AttachmentRules, SocketName);

	if (const auto ItemClass = GetClass())
	{
		if (const auto DefaultObject = Cast<AUpItem>(ItemClass->GetDefaultObject()))
		{
			SetActorScale3D(DefaultObject->GetActorRelativeScale3D());
		}
	}
}

void AUpItem::Detach()
{
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);

	if (const auto StaticMeshComponent = GetStaticMeshComponent())
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		StaticMeshComponent->SetEnableGravity(true);
		StaticMeshComponent->SetSimulatePhysics(true);
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
		ItemData = GameInstance->GetItemData(TagId);
	}

	check(ItemData.IsValid())
}
