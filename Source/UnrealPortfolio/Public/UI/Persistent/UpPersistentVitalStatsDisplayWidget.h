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
    void SetSecondary(const bool bInSecondary) { bSecondary = bInSecondary; }

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
    float Stamina = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float MaxStamina = 0.f;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UTexture2D> Image;
    
    UPROPERTY(BlueprintReadOnly)
    bool bSecondary = false;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

    UFUNCTION(BlueprintCallable)
    float GetHealthBarPercentage() const;
    UFUNCTION(BlueprintCallable)
    float GetShieldBarPercentage() const;
    UFUNCTION(BlueprintCallable)
    float GetStaminaBarPercentage() const;
	UFUNCTION(BlueprintCallable)
	ESlateVisibility HideIfSecondary() const;
	UFUNCTION(BlueprintCallable)
	ESlateVisibility ShowIfSecondary() const;

private:
    UPROPERTY(EditDefaultsOnly, Category="UP Params")
    float InterpSpeed = 3.f;
    
    UPROPERTY(Transient)
    TObjectPtr<AUpPlayableCharacter> Character;

    float TargetHealth = 0.f;
    float TargetShield = 0.f;
    float TargetStamina = 0.f;
    
    UFUNCTION()
    void HandleAttributeValueChange(const FGameplayTag& TagId, const FGameplayTag& AttributeTag, const float Value);
};
