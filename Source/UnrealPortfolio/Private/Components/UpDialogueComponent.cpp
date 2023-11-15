// Copyright AB. All Rights Reserved.

#include "Components/UpDialogueComponent.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/Player/UpPlayerCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/UpHud.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UUpDialogueComponent::UUpDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FUpDialogueData UUpDialogueComponent::GetAvailableDialogue() const
{
	for (const auto& DialogueData : AllDialogueData)
	{
		if (UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, DialogueData.TagRequirements)
			&& UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, DialogueData.TagProhibitions, true))
		{
			return DialogueData;
		}
	}

	return FUpDialogueData();
}

bool UUpDialogueComponent::HasAvailableDialogue() const
{
	return GetAvailableDialogue().IsValid();
}

void UUpDialogueComponent::StartDialogue(AUpPlayerController* PlayerController)
{
	if (!CustomOwner) return;

	if (const auto Player = PlayerController->GetCharacter())
	{
		// Make the actors face each other.
		const auto OwnerLocation = CustomOwner->GetActorLocation();
		const auto PlayerLocation = Player->GetActorLocation();
	
		// TODO(P1): Lerp the rotation.
		Player->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerLocation, OwnerLocation));
		CustomOwner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(OwnerLocation, PlayerLocation));

		if (const auto World = GetWorld())
		{
			// Create an over-the-shoulder dialogue camera.
			const auto CameraLocation = PlayerLocation + Player->GetActorForwardVector() * DialogueCameraOffsetForward +
				Player->GetActorRightVector() * DialogueCameraOffsetRight + Player->GetActorUpVector() * DialogueCameraOffsetUp;
			const auto CameraRotation = UKismetMathLibrary::FindLookAtRotation(
				CameraLocation, OwnerLocation + CustomOwner->GetActorUpVector() * DialogueCameraLookTargetOffsetUp);

			DialogueCamera = Cast<ACameraActor>(World->SpawnActor(ACameraActor::StaticClass(), &CameraLocation, &CameraRotation));

			if (DialogueCamera)
			{
				if (const auto DialogueCameraComponent = DialogueCamera->GetCameraComponent())
				{
					DialogueCameraComponent->SetConstraintAspectRatio(false);
				}
			
				PlayerController->SetViewTargetWithBlend(DialogueCamera, DialogueCameraBlendTime);
			}
		}
	}

	if (const auto Hud = PlayerController->GetCustomHud())
	{
		if (const auto DialogueData = GetAvailableDialogue(); DialogueData.IsValid())
		{
			FUpDialogueStepData FirstDialogueStep;
		
			for (const auto& Potential : DialogueData.PotentialFirstDialogueSteps)
			{
				if (const auto Data = *Potential.GetRow<FUpDialogueStepData>(TEXT("PotentialFirstDialogueStep GetRow"));
					UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, Data.TagRequirements) &&
					UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, Data.TagProhibitions, true))
				{
					FirstDialogueStep = Data;
					break;
				}
			}

			if (FirstDialogueStep.IsValid())
			{
				Hud->OpenDialogueFlow();
				Hud->DisplayDialogueStep(CustomOwner, FirstDialogueStep);
			}
		}
	}
}

void UUpDialogueComponent::EndDialogue(AUpPlayerController* PlayerController)
{
	if (const auto Hud = PlayerController->GetCustomHud())
	{
		Hud->CloseDialogueFlow();
	}

	if (const auto Player = PlayerController->GetCharacter())
	{
		PlayerController->SetViewTargetWithBlend(Player, DialogueCameraBlendTime, VTBlend_Linear, 0.f, true);
	}

	if (const auto World = GetWorld(); World && DialogueCamera)
	{
		World->DestroyActor(DialogueCamera);
		DialogueCamera = nullptr;
	}
}

void UUpDialogueComponent::AdvanceDialogueStep(const FUpDialogueStepData& DialogueStep, AUpPlayerController* PlayerController)
{
	UUpBlueprintFunctionLibrary::ProcessEntityTagSpecGrants(this, DialogueStep.TagGrants);
	
	if (DialogueStep.PotentialDialogueOptions.Num() > 0)
	{
		if (const auto Hud = PlayerController->GetCustomHud(); Hud && CustomOwner)
		{
			TArray<FUpDialogueOptionData> DialogueOptions;

			for (const auto& Potential : DialogueStep.PotentialDialogueOptions)
			{
				if (const auto Data = *Potential.GetRow<FUpDialogueOptionData>(TEXT("PotentialDialogueOption GetRow"));
					UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, Data.TagRequirements) &&
					UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, Data.TagProhibitions, true))
				{
					DialogueOptions.Add(Data);
				}
			}
		
			Hud->DisplayDialogueOptions(CustomOwner, DialogueOptions);
		}
	} else
	{
		EndDialogue(PlayerController);
	}
}

void UUpDialogueComponent::SelectDialogueOption(const FUpDialogueOptionData& DialogueOption, AUpPlayerController* PlayerController)
{
	FUpDialogueStepData NextDialogueStep;
		
	for (const auto& Potential : DialogueOption.PotentialNextDialogueSteps)
	{
		if (const auto Data = *Potential.GetRow<FUpDialogueStepData>(TEXT("PotentialNextDialogueStep GetRow"));
			UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, Data.TagRequirements) &&
			UUpBlueprintFunctionLibrary::IsEntityTagSpecSatisfied(this, Data.TagProhibitions, true))
		{
			NextDialogueStep = Data;
			break;
		}
	}

	if (NextDialogueStep.IsValid())
	{
		if (const auto Hud = PlayerController->GetCustomHud(); Hud && CustomOwner)
		{
			Hud->DisplayDialogueStep(CustomOwner, NextDialogueStep);
		}
	} else
	{
		EndDialogue(PlayerController);
	}
}

void UUpDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	check(DialogueDataTable);

	TArray<FUpDialogueData*> AllRows;
	DialogueDataTable->GetAllRows<FUpDialogueData>(TEXT("DialogueDataTable GetAllRows"), AllRows);

	for (const auto Row : AllRows)
	{
		AllDialogueData.Add(*Row);
	}
	
	CustomOwner = CastChecked<AUpNpcCharacter>(GetOwner());
}
