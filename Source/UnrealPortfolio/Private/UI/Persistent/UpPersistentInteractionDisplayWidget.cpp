// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentInteractionDisplayWidget.h"

#include "Interfaces/UpNameable.h"
#include "UI/UpHud.h"

ESlateVisibility UUpPersistentInteractionDisplayWidget::GetRootVisibility() const
{
	if (TargetInteractableName.IsEmpty()) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentInteractionDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);
	
	if (!CustomHud) return;
	
	CustomHud->TargetInteractableDelegate.AddUObject(this, &ThisClass::HandleTargetInteractableDelegate);
}

void UUpPersistentInteractionDisplayWidget::HandleTargetInteractableDelegate(const AActor* TargetInteractable)
{
	if (const auto Nameable = Cast<IUpNameable>(TargetInteractable))
	{
		TargetInteractableName = Nameable->GetInGameName();
	} else
	{
		TargetInteractableName = FText();
	}
}
