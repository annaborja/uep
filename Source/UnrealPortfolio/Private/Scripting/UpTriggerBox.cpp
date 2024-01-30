// Copyright AB. All Rights Reserved.

#include "Scripting/UpTriggerBox.h"

#include "Components/BoxComponent.h"

AUpTriggerBox::AUpTriggerBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(68.f, 136.f, 88.f));

	SetUpTriggerComponent(BoxComponent);
}

