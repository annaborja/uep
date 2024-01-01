// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentInteractionDisplayWidget.h"

#include "Components/RichTextBlock.h"
#include "UI/UpHud.h"

ESlateVisibility UUpPersistentInteractionDisplayWidget::GetRootVisibility() const
{
	if (!InteractionData.Interactable) return ESlateVisibility::Hidden;

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

	if (const auto Widget = GetPromptTextWidget())
	{
		Widget->SetText(InInteractionData.InteractionPromptText);
	}
	
	if (const auto Widget = GetPromptSubTextWidget())
	{
		Widget->SetText(InInteractionData.InteractionPromptSubText);
	}
}
