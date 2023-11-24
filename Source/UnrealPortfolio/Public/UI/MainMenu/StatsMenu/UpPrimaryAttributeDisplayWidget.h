// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPrimaryAttributeDisplayWidget.generated.h"

class UUpPrimaryAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API UUpPrimaryAttributeDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void PopulateAttribute(const FGameplayTag& Tag, const UUpPrimaryAttributeSet* AttributeSet);

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Name;
	UPROPERTY(BlueprintReadOnly)
	float Value;
};
