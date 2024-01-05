// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/UpPlayableCharacter.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentVitalStatsDisplayWidget.generated.h"

class AUpPlayableCharacter;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentVitalStatsDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
    void SetCharacter(AUpPlayableCharacter* InCharacter);

    FORCEINLINE AUpPlayableCharacter* GetCharacter() const { return Character; }

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
    UPROPERTY(Transient)
    TObjectPtr<AUpPlayableCharacter> Character;
    
    UFUNCTION()
    void HandleAttributeValueChange(const FGameplayTag& TagId, const FGameplayTag& AttributeTag, const float Value);
};
