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
		GrantQuest,
		PlayAnimation,
		PlayBark,
		SetBlackboardKey,
		SetPotentialLookTarget,
		ShowTutorial,
	};
}

USTRUCT()
struct FUpScriptCommand : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TriggerTag;
	UPROPERTY(EditDefaultsOnly)
	float Delay = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EUpScriptCommandType::Type> CommandType = EUpScriptCommandType::PlayBark;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NotifyTag;
	
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle DataRowHandle;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DataTag;
	UPROPERTY(EditDefaultsOnly)
	float RelevantFloat = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	FUpScriptActorParams ActorParamsA;
	UPROPERTY(EditDefaultsOnly)
	FUpScriptActorParams ActorParamsB;
	
	float DelayTimeElapsed = 0.f;
	bool bExecuted = false;
};

UCLASS()
class UNREALPORTFOLIO_API AUpLevelScriptActor : public ALevelScriptActor, public IUpInteractable
{
	GENERATED_BODY()

public:
	AUpLevelScriptActor();
	
	virtual FUpInteractionData GetInteractionData(const AController* Controller) override;
	virtual bool Interact(AController* Controller) override;
	virtual void OnInteractionEnd(AController* Controller) override;
	
	void NotifyTag(const FGameplayTag& Tag);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpScriptCommand"))
	TObjectPtr<UDataTable> ScriptCommandsTable;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Debug")
	bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LevelFadeInDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LookTargetAspectRatio = 21.f / 9.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LookTargetCameraBlendTime = 0.3f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float LookTargetFieldOfView = 60.f;

	bool bPotentialLookTargetActive = false;

	UPROPERTY(Transient)
	TArray<FUpScriptCommand> AllScriptCommands;
	UPROPERTY(Transient)
	TArray<FUpScriptCommand> CommandsToExecute;
	UPROPERTY(Transient)
	TObjectPtr<AActor> PotentialLookTarget;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void ExecuteCommand(const FUpScriptCommand& Command);
	void PlayAnimation(const FUpScriptCommand& Command) const;
	void PlayBark(const FUpScriptCommand& Command) const;
	void SetBlackboardKey(const FUpScriptCommand& Command) const;
	void SetPotentialLookTarget(const FUpScriptCommand& Command);
	void ShowTutorial(const FUpScriptCommand& Command) const;
};
