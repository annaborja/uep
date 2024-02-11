// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonActivatableWidget.h"
#include "UpObjectivesMenuWidget.generated.h"

class UUpObjectiveDetailedDisplayWidget;
class UUpObjectiveDisplayWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpObjectivesMenuWidget : public UUpCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnActivated() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetObjectivesContainer() const;
	UFUNCTION(BlueprintImplementableEvent)
	UUpObjectiveDetailedDisplayWidget* GetObjectiveDetailedDisplayWidget() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpObjectiveDisplayWidget> ObjectiveDisplayClass;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float ObjectiveGap = 4.f;
	
	void PopulateObjectives();
};
