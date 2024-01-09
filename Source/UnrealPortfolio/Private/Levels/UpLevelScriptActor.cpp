// Copyright AB. All Rights Reserved.

#include "Levels/UpLevelScriptActor.h"

#include "UpGameInstance.h"
#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tags/AnimationTags.h"
#include "UI/UpHud.h"
#include "Utils/Constants.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

void AUpLevelScriptActor::NotifyTag(const FGameplayTag& Tag)
{
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level tag notification: %s"), *Tag.ToString())
	}

	if (const auto Ptr = TagNotificationCommandsMap.Find(Tag))
	{
		CommandsToExecute.Append(Ptr->Commands);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Commands not found for tag %s"), *Tag.ToString())
	}
}

FUpInteractionData AUpLevelScriptActor::GetInteractionData(const AUpPlayerController* PlayerController)
{
	if (PotentialLookTarget) return FUpInteractionData(this, FText::FromString(TEXT("Look")));
	
	return FUpInteractionData();
}

void AUpLevelScriptActor::Interact(AUpPlayerController* PlayerController)
{
	if (PotentialLookTarget)
	{
		PlayerController->CreateTemporaryCamera(PotentialLookTarget, LookTargetCameraBlendTime, LookTargetAspectRatio, LookTargetFieldOfView);
	}
}

void AUpLevelScriptActor::OnInteractionEnd(AUpPlayerController* PlayerController)
{
	IUpInteractable::OnInteractionEnd(PlayerController);

	PlayerController->DestroyTemporaryCamera(LookTargetCameraBlendTime);
}

void AUpLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		GameInstance->SetLevelScriptActor(this);
	}

	if (const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		CameraManager->StartCameraFade(1.f, 0.f, LevelFadeInDuration, FLinearColor::Black);
	}
	
	CommandsToExecute.Append(LevelStartCommands);
}

void AUpLevelScriptActor::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<FUpScriptCommand> NewCommandsToExecute;

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
		UE_LOG(LogTemp, Warning, TEXT("Executing command: %d (%s)"), Command.CommandType, *Command.DataTag.ToString())
	}

	const auto CustomPlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	const auto CustomHud = CustomPlayerController ? CustomPlayerController->GetCustomHud() : nullptr;
	const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
	
	switch (Command.CommandType)
	{
	case EUpScriptCommandType::DisplayTutorial:
		if (GameInstance)
		{
			if (const auto& TutorialData = GameInstance->GetTutorialData(Command.DataTag); TutorialData.IsValid())
			{
				if (CustomHud) CustomHud->BroadcastTutorial(TutorialData);
			}
		}
		
		break;
	case EUpScriptCommandType::GrantQuest:
		UE_LOG(LogTemp, Warning, TEXT("Grant quest"))
		break;
	case EUpScriptCommandType::PlayAnimation:
		if (const auto Pawn = Cast<AUpCharacter>(FindActorWithTag(Command.RelevantClass, Command.DataTag)))
		{
			if (const auto Mesh = Pawn->GetMesh())
			{
				if (const auto AnimInstance = Mesh->GetAnimInstance())
				{
					UAnimMontage* Montage = nullptr;

					if (Command.DataTag_Secondary.MatchesTagExact(TAG_AnimationType_Gesture))
					{
						Montage = Pawn->GetGesturesMontage();
					}

					if (Montage)
					{
						AnimInstance->Montage_Play(Montage);

						if (Command.DataTag_Tertiary.IsValid())
						{
							TArray<FString> TagSegments;
							Command.DataTag_Tertiary.ToString().ParseIntoArray(TagSegments, TEXT("."));

							AnimInstance->Montage_JumpToSection(FName(TagSegments[2]), Montage);
						}
					}
				}
			}
		}
		
		break;
	case EUpScriptCommandType::PlayBark:
		if (const auto BarkDataPtr = BarkDataMap.Find(Command.DataTag))
		{
			auto BarkData = *BarkDataPtr;
			
			if (BarkData.bNotifyLevelOnEnd)
			{
				BarkData.NotifyTag = Command.DataTag;
			}
			
			if (CustomHud) CustomHud->BroadcastBark(BarkData);
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("Bark data not found for key %s"), *Command.DataTag.ToString())
		}
		
		break;
	case EUpScriptCommandType::SetPawnLookTarget:
		{
			if (const auto Pawn = Cast<AUpCharacter>(FindActorWithTag(Command.RelevantClass, Command.DataTag)))
			{
				if (const auto Target = FindActorWithTag(Command.RelevantClass_Secondary, Command.DataTag_Secondary))
				{
					if (const auto Controller = Cast<AUpAiController>(Pawn->GetController()))
					{
						if (const auto BlackboardComponent = Controller->GetBlackboardComponent())
						{
							BlackboardComponent->SetValueAsObject(FName(BLACKBOARD_SELECTOR_LOOK_TARGET), Target);
						}
					}
				}
			}
			
			break;
		}
	case EUpScriptCommandType::SetPawnMoveTarget:
		{
			if (const auto Pawn = Cast<AUpCharacter>(FindActorWithTag(Command.RelevantClass, Command.DataTag)))
			{
				if (const auto Target = FindActorWithTag(Command.RelevantClass_Secondary, Command.DataTag_Secondary))
				{
					if (const auto Controller = Cast<AUpAiController>(Pawn->GetController()))
					{
						if (const auto BlackboardComponent = Controller->GetBlackboardComponent())
						{
							BlackboardComponent->SetValueAsObject(FName(BLACKBOARD_SELECTOR_MOVE_TARGET), Target);
						}
					}
				}
			}
			
			break;
		}
	case EUpScriptCommandType::SetPotentialLookTarget:
		{
			if (!Command.DataTag.IsValid())
			{
				PotentialLookTarget = nullptr;
				
				if (CustomPlayerController)
				{
					if (const auto PossessedCharacter = CustomPlayerController->GetPossessedCharacter())
					{
						if (const auto InteractionComponent = PossessedCharacter->GetPlayerInteractionComponent())
						{
							InteractionComponent->SetInteractionData(GetInteractionData(CustomPlayerController));
						}
					}

					if (CustomPlayerController->GetActiveInteractable() == this)
					{
						OnInteractionEnd(CustomPlayerController);
					}
				}

				return;
			}

			if (const auto Actor = FindActorWithTag(Command.RelevantClass, Command.DataTag))
			{
				PotentialLookTarget = Actor;

				if (CustomPlayerController)
				{
					if (const auto PossessedCharacter = CustomPlayerController->GetPossessedCharacter())
					{
						if (const auto InteractionComponent = PossessedCharacter->GetPlayerInteractionComponent())
						{
							InteractionComponent->SetInteractionData(GetInteractionData(CustomPlayerController));
						}
					}
				}
			}
		
			break;
		}
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid script command type %d"), Command.CommandType)
	}
}

AActor* AUpLevelScriptActor::FindActorWithTag(const TSubclassOf<AActor> ActorClass, const FGameplayTag& TagId) const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ActorClass, OutActors);

	for (const auto Actor : OutActors)
	{
		if (const auto TagIdable = Cast<IUpTagIdable>(Actor); TagIdable && TagIdable->GetTagId().MatchesTagExact(TagId))
		{
			return Actor;
		}
	}

	return nullptr;
}
