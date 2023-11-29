// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "UpCombatComponent.generated.h"

class UGameplayAbility;

USTRUCT()
struct FUpHitBoxData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly)
	FVector StartLocationOffset = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditDefaultsOnly)
	FVector EndLocationOffset = FVector(0.f, 0.f, 0.f);
	
	UPROPERTY(EditDefaultsOnly)
	float HalfSize = 0.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static bool IsCombatAbility(const UGameplayAbility* Ability);
	
	UUpCombatComponent();
};
