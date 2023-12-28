// Copyright AB. All Rights Reserved.

#include "Items/UpWeapon.h"

#include "UpGameInstance.h"
#include "Components/SphereComponent.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpWeapon::AUpWeapon()
{
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(GetRootComponent());
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(TRACE_CHANNEL_INTERACTION, ECR_Block);
}

bool AUpWeapon::CanInteract() const
{
	return true;
}

void AUpWeapon::Interact(AUpPlayerController* PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("interact with wepon"))
}

void AUpWeapon::ToggleCastShadows(const bool bEnable) const
{
	TArray<UActorComponent*> WeaponComponents;
	GetComponents(UStaticMeshComponent::StaticClass(), WeaponComponents);

	for (const auto Component : WeaponComponents)
	{
		if (const auto StaticMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			StaticMeshComponent->SetCastShadow(bEnable);
		}
	}
}

void AUpWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		ItemData = GameInstance->GetItemData(TagId);
	}
}
