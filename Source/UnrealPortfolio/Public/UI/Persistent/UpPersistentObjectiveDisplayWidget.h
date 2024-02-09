// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "UpPersistentObjectiveDisplayWidget.generated.h"

struct FUpObjectiveData;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentObjectiveDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FText Text;

	UFUNCTION(BlueprintImplementableEvent)
	UWidgetAnimation* GetFadeAnimation() const;

	virtual void NativePreConstruct() override;
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	FTimerHandle HideTimerHandle;
	
	UFUNCTION()
	void HandleObjective(const FUpObjectiveData& ObjectiveData);
	UFUNCTION()
	void ClearData();
};
