// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentTutorialDisplayWidget.h"

#include "CommonActionWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "UI/UpHud.h"

void UUpPersistentTutorialDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UUpPersistentTutorialDisplayWidget::OnCustomHudSet_Implementation(AUpHud* NewCustomHud)
{
	Super::OnCustomHudSet_Implementation(NewCustomHud);

	if (!CustomHud) return;

	CustomHud->TutorialDelegate.AddUObject(this, &ThisClass::HandleTutorial);
}

void UUpPersistentTutorialDisplayWidget::HandleTutorial(const FUpTutorialData& TutorialData)
{
	Text = TutorialData.Text;

	if (const auto Widget = GetCommonActionWidget())
	{
		Widget->SetInputActions(TutorialData.InputActionRows);
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (const auto FadeAnimation = GetFadeAnimation())
	{
		PlayAnimationForward(FadeAnimation);
	}

	if (const auto World = GetWorld())
	{
		World->GetTimerManager().SetTimer(HideTimerHandle, this, &ThisClass::ClearData, TutorialData.TimeToLive);
	}
}

void UUpPersistentTutorialDisplayWidget::ClearData()
{
	if (const auto FadeAnimation = GetFadeAnimation())
	{
		const auto SequencePlayer = PlayAnimationReverse(FadeAnimation);
		SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& Player)
		{
			SetVisibility(ESlateVisibility::Hidden);

			Text = FText::GetEmpty();

			if (const auto Widget = GetCommonActionWidget())
			{
				Widget->SetInputActions(TArray<FDataTableRowHandle>());
			}
		});
	}
}
