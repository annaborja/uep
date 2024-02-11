// Copyright AB. All Rights Reserved.

#include "UI/MainMenu/JournalMenu/UpObjectivesMenuWidget.h"

#include "UpGameInstance.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBoxSlot.h"
#include "UI/UpHud.h"
#include "UI/MainMenu/JournalMenu/UpObjectiveDetailedDisplayWidget.h"
#include "UI/MainMenu/JournalMenu/UpObjectiveDisplayWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpObjectivesMenuWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulateObjectives();
}

void UUpObjectivesMenuWidget::PopulateObjectives()
{
	if (!CustomHud) return;
	
	const auto MissionMap = CustomHud->GetMissionMap();

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto Container = GetObjectivesContainer(); Container && ObjectiveDisplayClass)
		{
			Container->ClearChildren();
			uint8 WidgetIdx = 0;

			for (const auto& MissionMapEntry : MissionMap)
			{
				const auto MissionData = GameInstance->GetMissionData(MissionMapEntry.Key);

				if (!MissionData.IsValid()) continue;

				TArray<FString> TagSegments;
				MissionData.TagId.ToString().ParseIntoArray(TagSegments, TEXT("."));

				if (!TagSegments.IsValidIndex(1)) continue;

				for (int8 i = MissionMapEntry.Value.Step; i >= 0; i--)
				{
					const auto ObjectiveData = GameInstance->GetObjectiveData(FGameplayTag::RequestGameplayTag(
						FName(TEXT("Objective.") + TagSegments[1] + TEXT(".") + FString::FromInt(i))));
						
					if (!ObjectiveData.IsValid()) continue;
					
					const auto Widget = CreateWidget<UUpObjectiveDisplayWidget>(this, ObjectiveDisplayClass);
					Widget->SetCustomHud(CustomHud);
					Widget->SetMissionData(MissionData);
					Widget->SetObjectiveData(ObjectiveData);
					Widget->SetCompleted(MissionMapEntry.Value.Status != EUpMissionStatus::InProgress || MissionMapEntry.Value.Step > i);
					
					Widget->FocusDelegate.AddLambda([this, MissionData, ObjectiveData]
					{
						if (const auto DetailedDisplayWidget = GetObjectiveDetailedDisplayWidget())
						{
							DetailedDisplayWidget->SetMissionData(MissionData);
							DetailedDisplayWidget->SetObjectiveData(ObjectiveData);
						}
					});
					
					if (const auto PanelSlot = Cast<UScrollBoxSlot>(Container->AddChild(Widget));
						PanelSlot && WidgetIdx > 0)
					{
						PanelSlot->SetPadding(FMargin(0.f, ObjectiveGap, 0.f, 0.f));
					}

					if (WidgetIdx == 0) Widget->SetFocus();
					
					WidgetIdx++;
				}
			}
		}
	}
}
