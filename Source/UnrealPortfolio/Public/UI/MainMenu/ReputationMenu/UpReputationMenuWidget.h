// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpReputationMenuWidget.generated.h"

class UUpPlayerNpcReputationDisplayWidget;

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
	UPanelWidget* GetNpcReputationsContainer() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpPlayerNpcReputationDisplayWidget> NpcReputationDisplayWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float NpcReputationRowGap = 20.f;
	
	void PopulateNpcReputations();
};
