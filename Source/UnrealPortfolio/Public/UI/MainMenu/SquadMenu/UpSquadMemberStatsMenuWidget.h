// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tags/AttributeTags.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpSquadMemberStatsMenuWidget.generated.h"

struct FUpReputationData;
class UUpAbilitySystemComponent;
class UUpAttributeBarWidget;
class UUpProgressBarWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberStatsMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void PopulatePlayerSentiment(const FUpReputationData& PlayerReputationData);
	void SetAbilitySystemComponent(UUpAbilitySystemComponent* InAbilitySystemComponent);

protected:
	virtual void NativeOnActivated() override;

	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetPrimaryAttributesContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetPlayerSentimentAttributesContainer() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpAttributeBarWidget> PrimaryAttributeDisplayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpProgressBarWidget> SentimentAttributeDisplayWidgetClass;

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
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FText AffectionLabel = FText::FromString(TEXT("Affection"));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FText EsteemLabel = FText::FromString(TEXT("Esteem"));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float SentimentAttributeRowGap = 8.f;

	UPROPERTY(Transient)
	TObjectPtr<UUpAbilitySystemComponent> AbilitySystemComponent;

	void PopulatePrimaryAttributes();
};
