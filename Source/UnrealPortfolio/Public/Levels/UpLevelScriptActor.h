// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/UpDialogueComponent.h"
#include "Engine/LevelScriptActor.h"
#include "Interfaces/UpInteractable.h"
#include "UpLevelScriptActor.generated.h"

UENUM()
namespace EUpScriptCommandType
{
	enum Type : uint8
	{
		DisplayTutorial,
		ExecuteBark,
		GrantQuest,
		SetPotentialLookTarget
	};
}

USTRUCT()
struct FUpScriptCommand
{
	GENERATED_BODY()

	FUpScriptCommand() {}
	explicit FUpScriptCommand(const EUpScriptCommandType::Type InCommandType, const FGameplayTag& InDataTag) :
		CommandType(InCommandType), DataTag(InDataTag) {}

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpScriptCommandType::Type> CommandType = EUpScriptCommandType::ExecuteBark;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DataTag;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RelevantClass;
	
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
class UNREALPORTFOLIO_API AUpLevelScriptActor : public ALevelScriptActor, public IUpInteractable
{
	GENERATED_BODY()

public:
	void NotifyTag(const FGameplayTag& Tag);

	virtual FUpInteractionData GetInteractionData(const AUpPlayerController* PlayerController) override;
	virtual void Interact(AUpPlayerController* PlayerController) override;
	virtual void OnInteractionEnd(AUpPlayerController* PlayerController) override;
	
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
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LookTargetCameraBlendTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LookTargetAspectRatio = 21.f / 9.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LookTargetFieldOfView = 50.f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void ExecuteCommand(const FUpScriptCommand& Command);

private:
	UPROPERTY(Transient)
	TArray<FUpScriptCommand> CommandsToExecute;
	UPROPERTY(Transient)
	TObjectPtr<AActor> PotentialLookTarget;
};
