// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentInteractionDisplayWidget.h"

#include "Components/RichTextBlock.h"
#include "UI/UpHud.h"

ESlateVisibility UUpPersistentInteractionDisplayWidget::GetRootVisibility() const
{
	if (!bCanInteract) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

ESlateVisibility UUpPersistentInteractionDisplayWidget::GetImageVisibility() const
{
	if (!InteractionData.Image) return ESlateVisibility::Hidden;

	return ESlateVisibility::SelfHitTestInvisible;
}

void UUpPersistentInteractionDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);
	
	if (!CustomHud) return;
	
	CustomHud->InteractionDataDelegate.AddUObject(this, &ThisClass::HandleInteractionDataDelegate);
}

void UUpPersistentInteractionDisplayWidget::HandleInteractionDataDelegate(const FUpInteractionData InInteractionData)
{
	InteractionData = InInteractionData;
	bCanInteract = InInteractionData.Interactable != nullptr;

	if (const auto PromptTextWidget = GetPromptTextWidget())
	{
		PromptTextWidget->SetText(InInteractionData.InteractionPrompt);
	}
}
