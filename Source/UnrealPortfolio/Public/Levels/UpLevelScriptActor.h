// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpDialogueComponent.h"
#include "Engine/LevelScriptActor.h"
#include "UpLevelScriptActor.generated.h"

UENUM()
namespace EUpScriptCommandType
{
	enum Type : uint8
	{
		DisplayTutorial,
		ExecuteBark,
		GrantQuest
	};
}

USTRUCT()
struct FUpScriptCommand
{
	GENERATED_BODY()

	FUpScriptCommand() {}
	explicit FUpScriptCommand(const EUpScriptCommandType::Type InCommandType, const FGameplayTag& InDataKeyTag) :
		CommandType(InCommandType), DataKeyTag(InDataKeyTag) {}

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpScriptCommandType::Type> CommandType = EUpScriptCommandType::ExecuteBark;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DataKeyTag;
	
	UPROPERTY(EditDefaultsOnly)
	float Delay = 0.f;

	float DelayTimeElapsed = 0.f;
	bool bExecuted = false;
};

USTRUCT()
struct FUpScriptCommandList
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FUpScriptCommand> Commands;
};

UCLASS()
class UNREALPORTFOLIO_API AUpLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	void NotifyTag(const FGameplayTag& Tag);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(TitleProperty="CommandType"))
	TArray<FUpScriptCommand> LevelStartCommands;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(ForceInlineRow))
	TMap<FGameplayTag, FUpScriptCommandList> TagNotificationCommandsMap;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(ForceInlineRow))
	TMap<FGameplayTag, FUpBarkData> BarkDataMap;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Debug")
	bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LevelFadeInDuration = 1.f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void ExecuteCommand(const FUpScriptCommand& Command) const;

private:
	UPROPERTY(Transient)
	TArray<FUpScriptCommand> CommandsToExecute;
};
