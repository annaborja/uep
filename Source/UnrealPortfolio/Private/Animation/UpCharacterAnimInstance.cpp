// Copyright AB. All Rights Reserved.

#include "Animation/UpCharacterAnimInstance.h"

#include "Characters/UpCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UUpCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const auto Character = Cast<AUpCharacter>(TryGetPawnOwner()))
	{
		if (const auto MovementComponent = Character->GetCharacterMovement())
		{
			GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		}
	}
}
