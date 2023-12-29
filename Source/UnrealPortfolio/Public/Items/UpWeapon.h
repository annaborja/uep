// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpItem.h"
#include "UpWeapon.generated.h"

class UUpAmmoAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API AUpWeapon : public AUpItem
{
	GENERATED_BODY()

public:
	AUpWeapon();

protected:
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TSubclassOf<UGameplayEffect> InitAmmoAttributesEffectClass;
	
	UPROPERTY()
	TObjectPtr<UUpAmmoAttributeSet> AmmoAttributeSet;
	
	virtual void BeginPlay() override;
};
