// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonButtonBase.h"
#include "UpCharacterSwitcherButtonWidget.generated.h"

class AUpNpcCharacter;
struct FGameplayTag;

UENUM(BlueprintType)
namespace EUpCharacterSwitcherButtonState
{
	enum Type : uint8
	{
		Empty,
		Unavailable,
		Available,
		Active
	};
}

UCLASS()
class UNREALPORTFOLIO_API UUpCharacterSwitcherButtonWidget : public UUpCommonButtonBase
{
	GENERATED_BODY()

public:
	void SetNpc(AUpNpcCharacter* InNpc);
	void SetNpcTag(const FGameplayTag& NpcTag);

protected:
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EUpCharacterSwitcherButtonState::Type> CharacterSwitcherButtonState = EUpCharacterSwitcherButtonState::Empty;
	UPROPERTY(BlueprintReadOnly)
	FText Label;
	
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> Npc;
};
