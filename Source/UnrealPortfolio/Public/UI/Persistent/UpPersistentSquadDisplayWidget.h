// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentSquadDisplayWidget.generated.h"

class AUpPlayableCharacter;
class UUpPersistentVitalStatsDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentSquadDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
    virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

    UFUNCTION(BlueprintImplementableEvent)
    UUpPersistentVitalStatsDisplayWidget* GetVitalStatsDisplay_Main() const;
    UFUNCTION(BlueprintImplementableEvent)
    UUpPersistentVitalStatsDisplayWidget* GetVitalStatsDisplay_Secondary1() const;
    UFUNCTION(BlueprintImplementableEvent)
    UUpPersistentVitalStatsDisplayWidget* GetVitalStatsDisplay_Secondary2() const;

private:
    UFUNCTION()
    void HandlePossessedCharacterChange(AUpPlayableCharacter* PossessedCharacter);
    UFUNCTION()
    void HandleSecondarySquadMemberBroadcast(AUpPlayableCharacter* Character);

    TArray<UUpPersistentVitalStatsDisplayWidget*> GetSecondaryDisplayWidgets();
};
