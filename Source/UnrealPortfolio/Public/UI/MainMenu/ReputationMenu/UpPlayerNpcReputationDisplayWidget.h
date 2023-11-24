// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/Components/UpPlayerReputationComponent.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPlayerNpcReputationDisplayWidget.generated.h"

class UUpReputationAttributeDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPlayerNpcReputationDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetNpcName(const FText& InNpcName) { NpcName = InNpcName;  }
	
	void PopulateReputationData(const FUpReputationData& ReputationData);

	FORCEINLINE FText GetNpcName() const { return NpcName; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FText NpcName;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpReputationAttributeDisplayWidget> ReputationAttributeDisplayWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float ReputationAttributeRowGap = 8.f;

	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetReputationAttributesContainer() const;
};
