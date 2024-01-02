// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Enums.h"
#include "UpPersistentEquipmentDisplayWidget.generated.h"

struct FUpItemData;
class UUpPersistentEquipmentItemDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentEquipmentDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	TArray<UUpPersistentEquipmentItemDisplayWidget*> GetEquipmentItemDisplayWidgets() const;
	
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UFUNCTION()
	void HandleEquipmentActivationUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const bool bActivated);
	UFUNCTION()
	void HandleEquipmentUpdate(const EUpEquipmentSlot::Type EquipmentSlot, const FUpItemData& ItemData);
};
