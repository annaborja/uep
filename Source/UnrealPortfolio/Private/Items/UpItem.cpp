// Copyright AB. All Rights Reserved.

#include "Items/UpItem.h"

#include "UpGameInstance.h"
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

bool AUpItem::CanInteract() const
{
	return true;
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

void AUpItem::Interact(AUpPlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("interact with item"))
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
