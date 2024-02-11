// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Structs.h"
#include "UpObjectiveDetailedDisplayWidget.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpObjectiveDetailedDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetMissionData(const FUpMissionData InMissionData) { MissionData = InMissionData; }
	void SetObjectiveData(const FUpObjectiveData InObjectiveData) { ObjectiveData = InObjectiveData; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FUpMissionData MissionData;
	UPROPERTY(BlueprintReadOnly)
	FUpObjectiveData ObjectiveData;
};
