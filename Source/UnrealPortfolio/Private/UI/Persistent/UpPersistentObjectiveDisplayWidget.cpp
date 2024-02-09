// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentObjectiveDisplayWidget.h"

#include "UI/UpHud.h"

void UUpPersistentObjectiveDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UUpPersistentObjectiveDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->ObjectiveDelegate.AddUObject(this, &ThisClass::HandleObjective);
}

void UUpPersistentObjectiveDisplayWidget::HandleObjective(const FUpObjectiveData& ObjectiveData)
{
}

void UUpPersistentObjectiveDisplayWidget::ClearData()
{
}
