// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Utils/Structs.h"
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
	
	UPROPERTY(EditDefaultsOnly, meta=(RowType="/Script/UnrealPortfolio.UpDialogueStepData"))
	TArray<FDataTableRowHandle> PotentialFirstDialogueSteps;
};

USTRUCT()
struct FUpDialogueLine
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDialogueWave> DialogueWave;
	
	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty="Montage"))
	TArray<FUpMontageData> MontageData;
};

USTRUCT()
struct FUpDialogueStepData : public FTableRowBase
{
	GENERATED_BODY();
	
	bool IsValid() const { return DialogueLines.Num() > 0; }
	
	UPROPERTY(EditDefaultsOnly, meta=(TitleProperty="DialogueWave"))
	TArray<FUpDialogueLine> DialogueLines;
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagRequirements;
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagProhibitions;
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagGrants;
	
	UPROPERTY(EditDefaultsOnly, meta=(RowType="/Script/UnrealPortfolio.UpDialogueOptionData"))
	TArray<FDataTableRowHandle> PotentialDialogueOptions;
};

USTRUCT()
struct FUpDialogueOptionData : public FTableRowBase
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagRequirements;
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagProhibitions;

	UPROPERTY(EditDefaultsOnly)
	FText Text;
	
	UPROPERTY(EditDefaultsOnly)
	FUpEntityTagSpec TagGrants;
	
	UPROPERTY(EditDefaultsOnly, meta=(RowType="/Script/UnrealPortfolio.UpDialogueStepData"))
	TArray<FDataTableRowHandle> PotentialNextDialogueSteps;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpDialogueComponentDialogueEndedSignature, AAIController*, AiController);

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

	FUpDialogueComponentDialogueEndedSignature DialogueEndedDelegate;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets", meta=(RowType="/Script/UnrealPortfolio.UpDialogueData"))
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
