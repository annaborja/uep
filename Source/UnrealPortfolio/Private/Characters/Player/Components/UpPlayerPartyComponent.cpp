// Copyright AB. All Rights Reserved.

#include "Characters/Player/Components/UpPlayerPartyComponent.h"

#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UUpPlayerPartyComponent::SwitchCharacter(AUpPlayerController* PlayerController)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(PlayerController, AUpNpcCharacter::StaticClass(), OutActors);

	if (OutActors.IsValidIndex(0))
	{
		PlayerController->Possess(Cast<APawn>(OutActors[0]));
	}
}

UUpPlayerPartyComponent::UUpPlayerPartyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
