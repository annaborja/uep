// Copyright AB. All Rights Reserved.

#include "Items/UpDoor.h"

#include "Characters/UpPlayableCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Utils/Structs.h"

AUpDoor::AUpDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanInteract = true;

	InteractionSphere->SetRelativeLocation(InteractionSphere->GetRelativeLocation() + FVector(0.f, 50.f, 0.f));
	InteractionSphere->SetSphereRadius(70.f);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	if (const auto StaticMeshComponent = GetStaticMeshComponent())
	{
		StaticMeshComponent->SetCollisionProfileName(FName(TEXT("Custom")));
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

void AUpDoor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto CurrentRotation = GetActorRotation();
	const auto TargetRotation = bOpen ? Rotation_Open : Rotation_Closed;

	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s rotation current (%s), target (%s)"), *CurrentRotation.ToString(), *TargetRotation.ToString())
	}
	
	if (!bOpen && !bCloseSoundPlayed && CurrentRotation.Equals(TargetRotation, RotationTolerance_Closed))
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sfx_Close, GetActorLocation(), CurrentRotation, VolumeMultiplier_Closed);
		bCloseSoundPlayed = true;
	}

	if (CurrentRotation.Equals(TargetRotation, 0.1f))
	{
		SetActorTickEnabled(false);
		return;
	}
	
	SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, SwingSpeed));
}

void AUpDoor::Interact(AUpPlayerController* PlayerController)
{
	bPlayerHasInteracted = true;
	ToggleOpen();
}

void AUpDoor::BeginPlay()
{
	Super::BeginPlay();

	check(Sfx_Close);
	check(Sfx_Swing);

	SetActorTickEnabled(false);

	InteractionSphereBeginOverlapDelegate.BindUFunction(this, FName(TEXT("OnInteractionSphereBeginOverlap")));

	if (InteractionSphere) InteractionSphere->OnComponentBeginOverlap.Add(InteractionSphereBeginOverlapDelegate);
}

FText AUpDoor::GetInteractionPromptText(const AUpPlayerController* PlayerController) const
{
	return FText::FromString(FString::Printf(TEXT("%s %s"), bOpen ? TEXT("Close") : TEXT("Open"), *ItemData.Name.ToString()));
}

void AUpDoor::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bOpen || !bPlayerHasInteracted) return;
	
	if (const auto Character = Cast<AUpPlayableCharacter>(OtherActor); Character && !Character->IsPlayer())
	{
		ToggleOpen();
	}
}

void AUpDoor::ToggleOpen()
{
	bOpen = !bOpen;

	if (!bOpen)
	{
		bCloseSoundPlayed = false;
	}

	SetActorTickEnabled(true);
	
	UGameplayStatics::PlaySoundAtLocation(this, Sfx_Swing, GetActorLocation(), GetActorRotation());
}
