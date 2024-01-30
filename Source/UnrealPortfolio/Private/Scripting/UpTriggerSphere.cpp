// Copyright AB. All Rights Reserved.

#include "Scripting/UpTriggerSphere.h"

#include "Components/SphereComponent.h"

AUpTriggerSphere::AUpTriggerSphere()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(100.f);

	SetUpTriggerComponent(SphereComponent);
}
