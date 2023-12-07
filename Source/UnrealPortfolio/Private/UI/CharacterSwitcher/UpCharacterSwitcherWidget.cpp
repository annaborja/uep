// Copyright AB. All Rights Reserved.

#include "UI/CharacterSwitcher/UpCharacterSwitcherWidget.h"

#include "CommonButtonBase.h"
#include "EnhancedInputSubsystems.h"
#include "UpGameInstance.h"
#include "Characters/UpNpcCharacter.h"
#include "Characters/UpPlayableCharacter.h"
#include "Characters/Player/UpPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UpHud.h"
#include "UI/CharacterSwitcher/UpCharacterSwitcherButtonWidget.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

UWidget* UUpCharacterSwitcherWidget::NativeGetDesiredFocusTarget() const
{
	return GetDefaultFocusButton();
}

void UUpCharacterSwitcherWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulateCharacterSwitcherButtons();
}

FReply UUpCharacterSwitcherWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (CustomHud)
	{
		if (const auto PlayerController = CustomHud->GetCustomController())
		{
			if (const auto InputAction = PlayerController->GetCloseCharacterSwitcherInputAction())
			{
				if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
					Subsystem && Subsystem->QueryKeysMappedToAction(InputAction).Contains(InKeyEvent.GetKey()))
				{
					PlayerController->CloseCharacterSwitcher();
				}
			}
		}
	}
	
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void UUpCharacterSwitcherWidget::PopulateCharacterSwitcherButtons() const
{
	if (!CustomHud) return;

	if (const auto GameInstance = UUpBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (const auto PlayerController = CustomHud->GetCustomController())
		{
			if (const auto PossessedCharacter = PlayerController->GetPossessedCharacter())
			{
				FGameplayTagContainer OutTags;
				GameInstance->GetPartyMemberTags(OutTags);

				TArray<FGameplayTag> PartyMemberTags;
				OutTags.GetGameplayTagArray(PartyMemberTags);
		
				const auto CharacterSwitcherButtons = GetCharacterSwitcherButtons();
				const auto SpherePosition = PossessedCharacter->GetActorLocation();

				TArray<AActor*> OverlapActors;
				UKismetSystemLibrary::SphereOverlapActors(this, SpherePosition, OverlapSphereRadius,
					TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_Pawn) },
					AUpNpcCharacter::StaticClass(), TArray<AActor*> { PossessedCharacter }, OverlapActors);

				if (bDebugOverlapSphere)
				{
					UKismetSystemLibrary::DrawDebugSphere(this, SpherePosition, OverlapSphereRadius, 12, FColor::Orange, 5.f);
				}

				for (uint8 i = 0; CharacterSwitcherButtons.IsValidIndex(i); i++)
				{
					if (const auto Button = CharacterSwitcherButtons[i])
					{
						if (PartyMemberTags.IsValidIndex(i))
						{
							const auto PartyMemberTag = PartyMemberTags[i];
							AUpNpcCharacter* Npc = nullptr;

							for (const auto OverlapActor : OverlapActors)
							{
								if (const auto OverlapNpc = Cast<AUpNpcCharacter>(OverlapActor))
								{
									if (const auto TagIdable = Cast<IUpTagIdable>(OverlapNpc))
									{
										if (TagIdable->GetTagId().MatchesTagExact(PartyMemberTag))
										{
											Npc = OverlapNpc;
											break;
										}
									}
								}
							}

							if (Npc)
							{
								Button->SetNpc(Npc);
							} else
							{
								Button->SetNpcTag(PartyMemberTag);
							}
						} else
						{
							Button->SetNpc(nullptr);
						}
					}
				}
			}
		}
	}
}
