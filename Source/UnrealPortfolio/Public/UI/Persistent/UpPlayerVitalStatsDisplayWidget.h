// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPlayerVitalStatsDisplayWidget.generated.h"

struct FGameplayTag;

UCLASS()
class UNREALPORTFOLIO_API UUpPlayerVitalStatsDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	float Health = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth = 0.f;

	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

	UFUNCTION(BlueprintCallable)
	float GetHealthBarPercentage() const;

private:
	UFUNCTION()
	void HandleAttributeValueChange(const FGameplayTag& Tag, const float Value);
};
