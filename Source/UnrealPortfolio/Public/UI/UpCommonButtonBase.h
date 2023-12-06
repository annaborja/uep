// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Interfaces/UpHudSettable.h"
#include "UpCommonButtonBase.generated.h"

class AUpHud;

UCLASS()
class UNREALPORTFOLIO_API UUpCommonButtonBase : public UCommonButtonBase, public IUpHudSettable
{
	GENERATED_BODY()

public:
	virtual void SetCustomHud(AUpHud* InCustomHud) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AUpHud> CustomHud;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnCustomHudSet(AUpHud* NewCustomHud);
};
