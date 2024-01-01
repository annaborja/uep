// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerInteractionComponent.h"

#include "Characters/UpPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/UpInteractable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UpHud.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpPlayerInteractionComponent::UUpPlayerInteractionComponent()
{
	// TODO(P1): Optimize for performance (disable tick when not necessary).
	PrimaryComponentTick.bCanEverTick = true;
}

void UUpPlayerInteractionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PlayableCharacter || !PlayableCharacter->IsPlayer()) return;
	
	TArray<FHitResult> OutHits;
	
	if (const auto Capsule = PlayableCharacter->GetCapsuleComponent())
	{
		const auto OwnerLocation = PlayableCharacter->GetActorLocation();
	
		UKismetSystemLibrary::BoxTraceMulti(this,
			OwnerLocation, OwnerLocation + PlayableCharacter->GetActorForwardVector() * TraceLength,
			FVector(10.f, Capsule->GetUnscaledCapsuleRadius(), Capsule->GetUnscaledCapsuleHalfHeight()),
			PlayableCharacter->GetActorRotation(), UEngineTypes::ConvertToTraceType(TRACE_CHANNEL_INTERACTION), false,
			TArray<AActor*> { PlayableCharacter }, bDebugInteractionTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, OutHits, true);
	}

	FUpInteractionData NewInteractionData;

	for (const auto& Hit : OutHits) {
		if (const auto Interactable = Cast<IUpInteractable>(Hit.GetActor()))
		{
			if (const auto Data = Interactable->GetInteractionData(); Data.Interactable)
			{
				NewInteractionData = Data;
				break;
			}
		}
	}

	if (const auto Hud = UUpBlueprintFunctionLibrary::GetCustomHud(this); Hud && NewInteractionData != InteractionData)
	{
		Hud->BroadcastInteractionData(NewInteractionData);
	}

	InteractionData = NewInteractionData;
}

void UUpPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayableCharacter = CastChecked<AUpPlayableCharacter>(GetOwner());
}
