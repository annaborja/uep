// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentSpecialMoveDisplayWidget.h"

#include "UpGameInstance.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void UUpPersistentSpecialMoveDisplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateValues();
}

void UUpPersistentSpecialMoveDisplayWidget::UpdateValues()
{
	if (!CustomHud) return;

	if (const auto CustomController = CustomHud->GetCustomController())
	{
		if (const auto PossessedCharacter = CustomController->GetPossessedCharacter())
		{
			if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(PossessedCharacter))
			{
				SpecialMoveData = GameInstance->GetSpecialMoveData(PossessedCharacter->GetActiveSpecialMoveTag());
			}
		}
	}
}
