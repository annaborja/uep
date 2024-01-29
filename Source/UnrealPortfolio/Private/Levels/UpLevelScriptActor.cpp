// Copyright AB. All Rights Reserved.

#include "Levels/UpLevelScriptActor.h"

#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tags/ScriptTags.h"
#include "UI/UpHud.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpLevelScriptActor::AUpLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

FUpInteractionData AUpLevelScriptActor::GetInteractionData(const AUpPlayerController* PlayerController)
{
	if (PotentialLookTarget) return FUpInteractionData(this, FText::FromString(TEXT("Look")));
	
	return FUpInteractionData();
}

void AUpLevelScriptActor::Interact(AUpPlayerController* PlayerController)
{
	if (!PotentialLookTarget) return;
	
	PlayerController->CreateTemporaryCamera(PotentialLookTarget, LookTargetCameraBlendTime, LookTargetAspectRatio, LookTargetFieldOfView);
}

void AUpLevelScriptActor::OnInteractionEnd(AUpPlayerController* PlayerController)
{
	IUpInteractable::OnInteractionEnd(PlayerController);

	PlayerController->DestroyTemporaryCamera(LookTargetCameraBlendTime);
}

void AUpLevelScriptActor::NotifyTag(const FGameplayTag& Tag)
{
	TArray<FUpScriptCommand> TriggeredCommands;
	
	for (const auto& Command : AllScriptCommands)
	{
		if (Command.TriggerTag.MatchesTagExact(Tag)) TriggeredCommands.Add(Command);
	}

	CommandsToExecute.Append(TriggeredCommands);

	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level notify tag: %s, %d triggered commands"), *Tag.ToString(), TriggeredCommands.Num())
	}
}

void AUpLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	check(ScriptCommandsTable);

	TArray<FUpScriptCommand*> AllScriptCommandRows;
	ScriptCommandsTable->GetAllRows(TEXT("ScriptCommandsTable GetAllRows"), AllScriptCommandRows);

	for (const auto Row : AllScriptCommandRows)
	{
		AllScriptCommands.Add(*Row);
	}

	for (const auto& Command : AllScriptCommands)
	{
		// Commands with no trigger tag will be triggered automatically at the start of the level.
		if (!Command.TriggerTag.IsValid()) CommandsToExecute.Add(Command);
	}
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->SetLevelScriptActor(this);
	}

	if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		CameraManager->StartCameraFade(1.f, 0.f, LevelFadeInDuration, FLinearColor::Black);
	}
}

void AUpLevelScriptActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<FUpScriptCommand> NewCommandsToExecute;

	// Execute commands that are due to be executed and retain the rest for the next tick.
	for (auto& Command : CommandsToExecute)
	{
		if (Command.Delay > 0.f)
		{
			Command.DelayTimeElapsed += DeltaSeconds;

			if (Command.DelayTimeElapsed < Command.Delay)
			{
				NewCommandsToExecute.Add(Command);
				continue;
			}
		}
		
		ExecuteCommand(Command);
	}

	CommandsToExecute.Reset();
	CommandsToExecute.Append(NewCommandsToExecute);
}

void AUpLevelScriptActor::ExecuteCommand(const FUpScriptCommand& Command)
{
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Executing level script command: %d"), Command.CommandType)
	}
	
	switch (Command.CommandType)
	{
	case EUpScriptCommandType::GrantQuest:
		UE_LOG(LogTemp, Warning, TEXT("[Temp] Grant quest"))
		break;
	case EUpScriptCommandType::PlayAnimation:
		PlayAnimation(Command);
		break;
	case EUpScriptCommandType::PlayBark:
		PlayBark(Command);
		break;
	case EUpScriptCommandType::SetPawnFollowTarget:
		SetPawnFollowTarget(Command);
		break;
	case EUpScriptCommandType::SetPawnLookTarget:
		SetPawnLookTarget(Command);
		break;
	case EUpScriptCommandType::SetPawnMoveTarget:
		SetPawnMoveTarget(Command);
		break;
	case EUpScriptCommandType::SetPotentialLookTarget:
		SetPotentialLookTarget(Command);
		break;;
	case EUpScriptCommandType::ShowTutorial:
		ShowTutorial(Command);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid level script command type: %d"), Command.CommandType)
	}
}

void AUpLevelScriptActor::PlayAnimation(const FUpScriptCommand& Command) const
{
	if (const auto Actors = UUpBlueprintFunctionLibrary::FindActors(this, Command.ActorParamsA.ActorClass, Command.ActorParamsA.TagId)
		.FilterByPredicate([&Command](const AActor* Actor)
		{
			return UUpBlueprintFunctionLibrary::SatisfiesActorParams(Command.ActorParamsA, Actor);
		}); Actors.Num() > 0)
	{
		for (const auto Actor : Actors)
		{
			if (const auto Pawn = Cast<AUpCharacter>(Actor))
			{
				if (const auto Mesh = Pawn->GetMesh())
				{
					if (const auto AnimInstance = Mesh->GetAnimInstance())
					{
						UAnimMontage* Montage = nullptr;

						if (Command.DataTag.MatchesTag(TAG_Montage_Gestures))
						{
							Montage = Pawn->GetGesturesMontage();
						}

						if (Montage)
						{
							TArray<FString> TagSegments;
							Command.DataTag.ToString().ParseIntoArray(TagSegments, NAME_STRING_DELIMITER);

							AnimInstance->Montage_Play(Montage);

							if (TagSegments.IsValidIndex(2))
							{
								AnimInstance->Montage_JumpToSection(FName(TagSegments[2]), Montage);
							}
					
							continue;
						}
					}
				}
			}
		
			UE_LOG(LogTemp, Error, TEXT("Play animation failed: %s / %s"), *Command.ActorParamsA.TagId.ToString(), *Command.DataTag.ToString())
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Play animation no actors found: %s / %s"), *Command.ActorParamsA.TagId.ToString(), *Command.DataTag.ToString())
	}
}

void AUpLevelScriptActor::PlayBark(const FUpScriptCommand& Command) const
{
	if (const auto Data = Command.DataRowHandle.GetRow<FUpBarkData>(TEXT("FUpBarkData GetRow")); Data && Data->IsValid())
	{
		if (Command.NotifyTag.IsValid())
		{
			Data->NotifyTag = Command.NotifyTag;
		}

		if (const auto CustomHud = UUpBlueprintFunctionLibrary::GetCustomHud(this))
		{
			CustomHud->BroadcastBark(*Data);
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Bark data not found: %s"), *Command.DataRowHandle.ToDebugString())
	}
}

void AUpLevelScriptActor::SetPawnFollowTarget(const FUpScriptCommand& Command) const
{
	SetBlackboardKey(Command, FName(BLACKBOARD_SELECTOR_FOLLOW_ACTOR));
}

void AUpLevelScriptActor::SetPawnLookTarget(const FUpScriptCommand& Command) const
{
	SetBlackboardKey(Command, FName(BLACKBOARD_SELECTOR_LOOK_TARGET_LOCATION));
}

void AUpLevelScriptActor::SetPawnMoveTarget(const FUpScriptCommand& Command) const
{
	SetBlackboardKey(Command, FName(Command.bUseTransform ? BLACKBOARD_SELECTOR_MOVE_TARGET_LOCATION : BLACKBOARD_SELECTOR_MOVE_TARGET_ACTOR));
}

void AUpLevelScriptActor::SetPotentialLookTarget(const FUpScriptCommand& Command)
{
	if (Command.ActorParamsA.TagId.IsValid())
	{
		if (const auto Actors = UUpBlueprintFunctionLibrary::FindActors(this, Command.ActorParamsA.ActorClass, Command.ActorParamsA.TagId)
			.FilterByPredicate([&Command](const AActor* Actor)
			{
				return UUpBlueprintFunctionLibrary::SatisfiesActorParams(Command.ActorParamsA, Actor);
			}); Actors.IsValidIndex(0))
		{
			if (const auto Actor = Actors[0])
			{
				PotentialLookTarget = Actor;

				if (const auto CustomPlayerController = UUpBlueprintFunctionLibrary::GetCustomPlayerController(this))
				{
					if (const auto PlayerPossessedCharacter = CustomPlayerController->GetPossessedCharacter())
					{
						if (const auto InteractionComponent = PlayerPossessedCharacter->GetPlayerInteractionComponent())
						{
							InteractionComponent->SetInteractionData(GetInteractionData(CustomPlayerController));
							return;
						}
					}
				}
			}
		}
		
		UE_LOG(LogTemp, Error, TEXT("Set potential look target failed: %s"), *Command.ActorParamsA.TagId.ToString())
	} else
	{
		PotentialLookTarget = nullptr;
				
		if (const auto CustomPlayerController = UUpBlueprintFunctionLibrary::GetCustomPlayerController(this))
		{
			if (const auto PlayerPossessedCharacter = CustomPlayerController->GetPossessedCharacter())
			{
				if (const auto InteractionComponent = PlayerPossessedCharacter->GetPlayerInteractionComponent())
				{
					InteractionComponent->SetInteractionData(GetInteractionData(CustomPlayerController));
				}
			}

			if (CustomPlayerController->GetActiveInteractable() == this)
			{
				OnInteractionEnd(CustomPlayerController);
			}
		}
	}
}

void AUpLevelScriptActor::ShowTutorial(const FUpScriptCommand& Command) const
{
	if (const auto Data = Command.DataRowHandle.GetRow<FUpTutorialData>(TEXT("FUpTutorialData GetRow")); Data && Data->IsValid())
	{
		if (const auto CustomHud = UUpBlueprintFunctionLibrary::GetCustomHud(this))
		{
			CustomHud->BroadcastTutorial(*Data);
		}
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Tutorial data not found: %s"), *Command.DataRowHandle.ToDebugString())
	}
}

void AUpLevelScriptActor::SetBlackboardKey(const FUpScriptCommand& Command, const FName& BlackboardKey) const
{
	const auto Actors = UUpBlueprintFunctionLibrary::FindActors(this, Command.ActorParamsA.ActorClass, Command.ActorParamsA.TagId)
		.FilterByPredicate([&Command](const AActor* Actor)
		{
			return UUpBlueprintFunctionLibrary::SatisfiesActorParams(Command.ActorParamsA, Actor);
		});
	const auto Targets = UUpBlueprintFunctionLibrary::FindActors(this, Command.ActorParamsB.ActorClass, Command.ActorParamsB.TagId)
		.FilterByPredicate([&Command](const AActor* Actor)
		{
			return UUpBlueprintFunctionLibrary::SatisfiesActorParams(Command.ActorParamsB, Actor);
		});
	
	if (Actors.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Set blackboard key no actors found: %s / %s"), *Command.ActorParamsA.TagId.ToString(), *Command.ActorParamsB.TagId.ToString())
	} else if (Targets.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Set blackboard key no targets found: %s / %s"), *Command.ActorParamsA.TagId.ToString(), *Command.ActorParamsB.TagId.ToString())
	} else
	{
		for (const auto Actor : Actors)
		{
			if (const auto Pawn = Cast<AUpCharacter>(Actor))
			{
				if (const auto PawnController = Cast<AUpAiController>(Pawn->GetController()))
				{
					if (const auto BlackboardComponent = PawnController->GetBlackboardComponent())
					{
						for (const auto Target : Targets)
						{
							if (Command.bUseTransform)
							{
								BlackboardComponent->SetValueAsVector(BlackboardKey, Target->GetActorLocation());
							} else
							{
								BlackboardComponent->SetValueAsObject(BlackboardKey, Target);
							}
						}
						
						continue;
					}
				}
			}
			
			UE_LOG(LogTemp, Error, TEXT("Set blackboard key failed: %s / %s"), *Command.ActorParamsA.TagId.ToString(), *Command.ActorParamsB.TagId.ToString())
		}
	}
}
