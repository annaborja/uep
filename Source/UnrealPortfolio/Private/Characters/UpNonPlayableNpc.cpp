// Copyright AB. All Rights Reserved.

#include "Characters/UpNonPlayableNpc.h"

#include "Components/UpCharacterMovementComponent.h"

AUpNonPlayableNpc::AUpNonPlayableNpc(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UUpCharacterMovementComponent>(CharacterMovementComponentName)) {}
