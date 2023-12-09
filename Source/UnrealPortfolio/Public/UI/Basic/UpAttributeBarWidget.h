// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpAttributeBarWidget.generated.h"

struct FGameplayTag;
class UUpAttributeSet;

UCLASS()
class UNREALPORTFOLIO_API UUpAttributeBarWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void PopulateAttribute(const FGameplayTag& Tag, const UUpAttributeSet* AttributeSet);

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Name;
	UPROPERTY(BlueprintReadOnly)
	float Value;
};
