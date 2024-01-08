// Copyright AB. All Rights Reserved.

#include "Levels/UpLevelScriptActor.h"

#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/Components/UpPlayerInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpHud.h"
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
			
			TArray<AActor*> OutActors;
			UGameplayStatics::GetAllActorsOfClass(this, Command.RelevantClass, OutActors);

			for (const auto Actor : OutActors)
			{
				if (const auto TagIdable = Cast<IUpTagIdable>(Actor); TagIdable && TagIdable->GetTagId().MatchesTagExact(Command.DataTag))
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

					return;
				}
			}
		
			break;
		}
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid script command type %d"), Command.CommandType)
	}
}
