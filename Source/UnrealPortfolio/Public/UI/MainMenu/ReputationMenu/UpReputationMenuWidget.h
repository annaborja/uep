// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpReputationMenuWidget.generated.h"

class UUpPlayerNpcReputationDataWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpReputationMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;

	UPROPERTY(BlueprintReadOnly)
	FUpReputationData PlayerWorldReputationData;
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, FUpReputationData> PlayerNpcReputationDataMap;

	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetNpcReputationDataContainer() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpPlayerNpcReputationDataWidget> NpcReputationDataWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float ReputationDataRowGap = 20.f;
	
	void PopulateNpcReputationData();
};
