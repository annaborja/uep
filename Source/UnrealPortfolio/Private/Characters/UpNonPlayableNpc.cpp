// Copyright AB. All Rights Reserved.

#include "Characters/UpNonPlayableNpc.h"

#include "Components/CapsuleComponent.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpNonPlayableNpc::AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName))
{
	AimAssistCapsule_Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AimAssistCapsule_Body"));
	AimAssistCapsule_Body->SetupAttachment(GetRootComponent());
	AimAssistCapsule_Body->SetCollisionResponseToAllChannels(ECR_Ignore);
	AimAssistCapsule_Body->SetCollisionResponseToChannel(TRACE_CHANNEL_AIM_ASSIST, ECR_Block);
	
	AimAssistCapsule_Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AimAssistCapsule_Head"));
	AimAssistCapsule_Head->SetupAttachment(GetRootComponent());
	AimAssistCapsule_Head->SetCollisionResponseToAllChannels(ECR_Ignore);
	AimAssistCapsule_Head->SetCollisionResponseToChannel(TRACE_CHANNEL_AIM_ASSIST, ECR_Block);
	
	if (const auto Capsule = GetCapsuleComponent())
	{
		if (AimAssistCapsule_Body)
		{
			AimAssistCapsule_Body->SetCapsuleHalfHeight(Capsule->GetUnscaledCapsuleHalfHeight() - AimAssistHeadHeight);
			AimAssistCapsule_Body->SetCapsuleRadius(Capsule->GetUnscaledCapsuleRadius());
			AimAssistCapsule_Body->SetRelativeLocation(FVector(0.f, 0.f,  -AimAssistHeadHeight));
		}

		if (AimAssistCapsule_Head)
		{
			AimAssistCapsule_Head->SetCapsuleHalfHeight(Capsule->GetUnscaledCapsuleHalfHeight());
			AimAssistCapsule_Head->SetCapsuleRadius(AimAssistHeadRadius);
		}
	}
}

void AUpNonPlayableNpc::Die()
{
	Super::Die();

	if (AimAssistCapsule_Body) UUpBlueprintFunctionLibrary::TearDownActorComponent(AimAssistCapsule_Body);
	if (AimAssistCapsule_Head) UUpBlueprintFunctionLibrary::TearDownActorComponent(AimAssistCapsule_Head);
}
