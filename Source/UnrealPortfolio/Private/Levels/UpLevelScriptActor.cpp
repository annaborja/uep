// Copyright AB. All Rights Reserved.

#include "Levels/UpLevelScriptActor.h"

#include "UpGameInstance.h"
#include "Characters/Player/UpPlayerController.h"
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

void AUpLevelScriptActor::ExecuteCommand(const FUpScriptCommand& Command) const
{
	if (bDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Executing command: %d (%s)"), Command.CommandType, *Command.DataKeyTag.ToString())
	}

	const auto CustomPlayerController = Cast<AUpPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	const auto CustomHud = CustomPlayerController ? CustomPlayerController->GetCustomHud() : nullptr;
	const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this);
	
	switch (Command.CommandType)
	{
	case EUpScriptCommandType::DisplayTutorial:
		if (GameInstance)
		{
			if (const auto& TutorialData = GameInstance->GetTutorialData(Command.DataKeyTag); TutorialData.IsValid())
			{
				if (CustomHud) CustomHud->BroadcastTutorial(TutorialData);
			}
		}
		
		break;
	case EUpScriptCommandType::ExecuteBark:
		if (const auto BarkDataPtr = BarkDataMap.Find(Command.DataKeyTag))
		{
			auto BarkData = *BarkDataPtr;
			
			if (BarkData.bNotifyLevelOnEnd)
			{
				BarkData.NotifyTag = Command.DataKeyTag;
			}
			
			if (CustomHud) CustomHud->BroadcastBark(BarkData);
		} else
		{
			UE_LOG(LogTemp, Error, TEXT("Bark data not found for key %s"), *Command.DataKeyTag.ToString())
		}
		
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid script command type %d"), Command.CommandType)
	}
}
