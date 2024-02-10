// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Tags/AttributeTags.h"
#include "UI/UpCommonActivatableWidget.h"
#include "Utils/Enums.h"
#include "UpSquadMemberStatsMenuWidget.generated.h"

struct FUpCharacterEquipment;
class UUpSquadMemberEquipmentItemDisplayWidget;
class AUpPlayableNpc;
class UUpAttributeBarWidget;
class UUpProgressBarWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpSquadMemberStatsMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetNpc(const AUpPlayableNpc* Npc);
	void SetNpcTagId(const FGameplayTag& NpcTagId);

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bHasFullData = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetPrimaryAttributesContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetPlayerSentimentAttributesContainer() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetWeapon1Display() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetWeapon2Display() const;
	
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetHelmetDisplay() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpSquadMemberEquipmentItemDisplayWidget* GetArmorDisplay() const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpAttributeBarWidget> PrimaryAttributeDisplayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpProgressBarWidget> SentimentAttributeDisplayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	TArray<FGameplayTag> PrimaryAttributeTags {
		TAG_Attribute_Primary_Resilience,
		TAG_Attribute_Primary_Speed,
		TAG_Attribute_Primary_Dexterity,
		TAG_Attribute_Primary_Intelligence,
		TAG_Attribute_Primary_Instinct
	};
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float PrimaryAttributeRowGap = 32.f;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FText AffectionLabel = FText::FromString(TEXT("Affection"));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FText EsteemLabel = FText::FromString(TEXT("Esteem"));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float SentimentAttributeRowGap = 12.f;

	void PopulatePrimaryAttributes(const AUpPlayableNpc* Npc);
	void PopulatePlayerSentiment(const FGameplayTag& NpcTagId);
	static void PopulateEquipmentItemDisplay(UUpSquadMemberEquipmentItemDisplayWidget* Widget, const FUpCharacterEquipment& Equipment, const EUpEquipmentSlot::Type EquipmentSlot);
};
