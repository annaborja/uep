// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tags/AttributeTags.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpStatsMenuWidget.generated.h"

struct FGameplayTag;
class UUpPrimaryAttributeDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpStatsMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;

	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetPrimaryAttributesContainer() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpPrimaryAttributeDisplayWidget> PrimaryAttributeDisplayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	TArray<FGameplayTag> PrimaryAttributeTags {
		TAG_Attribute_Primary_Resilience,
		TAG_Attribute_Primary_Strength,
		TAG_Attribute_Primary_Speed,
		TAG_Attribute_Primary_Dexterity,
		TAG_Attribute_Primary_Intelligence,
		TAG_Attribute_Primary_Instinct
	};
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float PrimaryAttributeRowGap = 20.f;

	void PopulatePrimaryAttributes();
};
