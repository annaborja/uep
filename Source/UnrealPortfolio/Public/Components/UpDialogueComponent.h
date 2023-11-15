// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Utils/GeneralStructs.h"
#include "UpDialogueComponent.generated.h"

class AUpPlayerController;
class AUpNpcCharacter;
class AUpPlayerCharacter;
class UDialogueWave;

USTRUCT()
struct FUpDialogueData : public FTableRowBase
{
	GENERATED_BODY();

	bool IsValid() const { return PotentialFirstDialogueSteps.Num() > 0; }
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagRequirements;
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagProhibitions;
	
	UPROPERTY(EditDefaultsOnly, meta=(RowType="UpDialogueStepData"))
	TArray<FDataTableRowHandle> PotentialFirstDialogueSteps;
};

USTRUCT(BlueprintType)
struct FUpDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDialogueWave> DialogueWave;
	
	UPROPERTY(EditDefaultsOnly)
	FUpMontageData MontageData;
};

USTRUCT(BlueprintType)
struct FUpDialogueStepData : public FTableRowBase
{
	GENERATED_BODY();
	
	bool IsValid() const { return DialogueLines.Num() > 0; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="DialogueWave"))
	TArray<FUpDialogueLine> DialogueLines;
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagRequirements;
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagProhibitions;
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagGrants;
	
	UPROPERTY(EditDefaultsOnly, meta=(RowType="UpDialogueOptionData"))
	TArray<FDataTableRowHandle> PotentialDialogueOptions;
};

USTRUCT(BlueprintType)
struct FUpDialogueOptionData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagRequirements;
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagProhibitions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Text;
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagGrants;
	
	UPROPERTY(EditDefaultsOnly, meta=(RowType="UpDialogueStepData"))
	TArray<FDataTableRowHandle> PotentialNextDialogueSteps;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPORTFOLIO_API UUpDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUpDialogueComponent();

	FUpDialogueData GetAvailableDialogue() const;
	bool HasAvailableDialogue() const;
	
	void StartDialogue(AUpPlayerController* PlayerController);
	void EndDialogue(AUpPlayerController* PlayerController);
	
	void AdvanceDialogueStep(const FUpDialogueStepData& DialogueStep, AUpPlayerController* PlayerController);
	void SelectDialogueOption(const FUpDialogueOptionData& DialogueOption, AUpPlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="UpDialogueData"))
	TObjectPtr<UDataTable> DialogueDataTable;
	
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float DialogueCameraBlendTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float DialogueCameraOffsetForward = -40.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float DialogueCameraOffsetRight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float DialogueCameraOffsetUp = 70.f;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float DialogueCameraLookTargetOffsetUp = 50.f;
	
	UPROPERTY(Transient)
	TArray<FUpDialogueData> AllDialogueData;
	UPROPERTY(Transient)
	TObjectPtr<AUpNpcCharacter> CustomOwner;
	UPROPERTY(Transient)
	TObjectPtr<ACameraActor> DialogueCamera;
};
