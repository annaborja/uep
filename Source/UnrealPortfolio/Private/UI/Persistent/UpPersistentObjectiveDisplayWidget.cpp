// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentObjectiveDisplayWidget.h"

#include "Animation/UMGSequencePlayer.h"
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
	Text = ObjectiveData.Summary;

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (const auto FadeAnimation = GetFadeAnimation())
	{
		PlayAnimationForward(FadeAnimation);
	}

	if (const auto World = GetWorld())
	{
		World->GetTimerManager().SetTimer(HideTimerHandle, this, &ThisClass::ClearData, 6.f);
	}
}

void UUpPersistentObjectiveDisplayWidget::ClearData()
{
	if (const auto FadeAnimation = GetFadeAnimation())
	{
		const auto SequencePlayer = PlayAnimationReverse(FadeAnimation);
		SequencePlayer->OnSequenceFinishedPlaying().AddLambda([this](const UUMGSequencePlayer& Player)
		{
			SetVisibility(ESlateVisibility::Collapsed);

			Text = FText::GetEmpty();
		});
	}
}
