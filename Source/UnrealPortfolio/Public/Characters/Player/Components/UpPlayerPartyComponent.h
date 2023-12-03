// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpPlayerPartyComponent.generated.h"

class AUpPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpPlayerPartyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	static void SwitchCharacter(AUpPlayerController* PlayerController);
	
	UUpPlayerPartyComponent();
};
