// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpTargetInteractableDisplayWidget.h"

#include "Interfaces/UpNameable.h"
#include "UI/UpHud.h"

ESlateVisibility UUpTargetInteractableDisplayWidget::GetRootVisibility() const
{
	if (TargetInteractableName.IsEmpty()) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpTargetInteractableDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);
	
	CustomHud->TargetInteractableDelegate.AddDynamic(this, &ThisClass::HandleTargetInteractableDelegate);
}

void UUpTargetInteractableDisplayWidget::HandleTargetInteractableDelegate(const AActor* TargetInteractable)
{
	if (const auto Nameable = Cast<IUpNameable>(TargetInteractable))
	{
		TargetInteractableName = Nameable->GetInGameName();
	} else
	{
		TargetInteractableName = FText();
	}
}
