// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerInteractionComponent.h"

#include "Characters/UpPlayableCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/UpInteractable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Levels/UpLevelScriptActor.h"
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

	// Level script "interactions" take precedence over all other interactions.
	if (Cast<AUpLevelScriptActor>(InteractionData.Interactable)) return;
	
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

	if (const auto CustomController = PlayableCharacter->GetCustomPlayerController())
	{
		for (const auto& Hit : OutHits) {
			if (const auto Interactable = Cast<IUpInteractable>(Hit.GetActor()))
			{
				if (const auto Data = Interactable->GetInteractionData(CustomController); Data.Interactable)
				{
					NewInteractionData = Data;
					break;
				}
			}
		}
		
		if (const auto CustomHud = CustomController->GetCustomHud(); CustomHud && NewInteractionData != InteractionData)
		{
			CustomHud->BroadcastInteractionData(NewInteractionData);
		}
	}

	InteractionData = NewInteractionData;
}

void UUpPlayerInteractionComponent::SetInteractionData(const FUpInteractionData& InInteractionData)
{
	InteractionData = InInteractionData;

	if (const auto CustomHud = UUpBlueprintFunctionLibrary::GetCustomHud(this))
	{
		CustomHud->BroadcastInteractionData(InteractionData);
	}
}

void UUpPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayableCharacter = CastChecked<AUpPlayableCharacter>(GetOwner());
}
