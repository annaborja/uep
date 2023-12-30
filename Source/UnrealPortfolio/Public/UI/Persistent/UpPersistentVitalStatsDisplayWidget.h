// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentVitalStatsDisplayWidget.generated.h"

class AUpPlayableCharacter;
struct FGameplayTag;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentVitalStatsDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(BlueprintReadOnly)
    float Health = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float MaxHealth = 0.f;
    
    UPROPERTY(BlueprintReadOnly)
    float Shield = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float MaxShield = 0.f;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UTexture2D> Image;

    virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

    UFUNCTION(BlueprintCallable)
    float GetHealthBarPercentage() const;
    UFUNCTION(BlueprintCallable)
    float GetShieldBarPercentage() const;

private:
    UFUNCTION()
    void HandleAttributeValueChange(const FGameplayTag& Tag, const float Value);
    UFUNCTION()
    void HandlePossessedCharacterChange(const AUpPlayableCharacter* PossessedCharacter);

    void InitAttributes(const AUpPlayableCharacter* PossessedCharacter);
};
