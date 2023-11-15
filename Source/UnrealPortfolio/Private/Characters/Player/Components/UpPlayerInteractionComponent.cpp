// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerInteractionComponent.h"

#include "Characters/Player/UpPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/UpInteractable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UpHud.h"
#include "Utils/Constants.h"

UUpPlayerInteractionComponent::UUpPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUpPlayerInteractionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CustomOwner) return;
	
	TArray<FHitResult> OutHits;
	
	if (const auto Capsule = CustomOwner->GetCapsuleComponent())
	{
		const auto OwnerLocation = CustomOwner->GetActorLocation();
	
		UKismetSystemLibrary::BoxTraceMulti(this,
			OwnerLocation, OwnerLocation + CustomOwner->GetActorForwardVector() * TraceLength,
			FVector(10.f, Capsule->GetUnscaledCapsuleRadius(), Capsule->GetUnscaledCapsuleHalfHeight()),
			CustomOwner->GetActorRotation(), UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_INTERACTION), false,
			TArray<AActor*> { CustomOwner }, bDebugInteractionTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, OutHits, true);
	}

	FHitResult InteractableHit;

	for (const auto& Hit : OutHits) {
		if (const auto Interactable = Cast<IUpInteractable>(Hit.GetActor()); Interactable && Interactable->CanInteract())
		{
			InteractableHit = Hit;
			break;
		}
	}

	const auto NewTargetInteractable = InteractableHit.GetActor();

	if (const auto Hud = CustomOwner->GetCustomHud(); NewTargetInteractable != TargetInteractable)
	{
		Hud->BroadcastTargetInteractable(NewTargetInteractable);
	}

	TargetInteractable = NewTargetInteractable;
}

void UUpPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	CustomOwner = CastChecked<AUpPlayerCharacter>(GetOwner());
}
