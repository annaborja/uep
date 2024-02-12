// Copyright AB. All Rights Reserved.

#include "UI/PowerWheel/UpPowerWheelWidget.h"

#include "CommonButtonBase.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/UpPlayableCharacter.h"
#include "Characters/UpPlayableNpc.h"
#include "Characters/Player/UpPlayerController.h"
#include "Characters/Player/UpPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UpHud.h"
#include "UI/PowerWheel/UpPowerWheelButtonWidget.h"

void UUpPowerWheelWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	PopulatePowerWheelButtons();
}

void UUpPowerWheelWidget::PopulatePowerWheelButtons() const
{
	if (!CustomHud) return;

	if (const auto CustomController = CustomHud->GetCustomController())
	{
		if (const auto CustomPlayerState = CustomController->GetPlayerState<AUpPlayerState>())
		{
			if (const auto PossessedCharacter = CustomController->GetPossessedCharacter())
			{
				FGameplayTagContainer OutTags;
				CustomPlayerState->GetSquadMemberTags(OutTags);

				TArray<FGameplayTag> SquadMemberTags;
				OutTags.GetGameplayTagArray(SquadMemberTags);
	
				const auto PowerWheelButtons = GetPowerWheelButtons();
				const auto SpherePosition = PossessedCharacter->GetActorLocation();

				TArray<AActor*> OverlapActors;
				UKismetSystemLibrary::SphereOverlapActors(this, SpherePosition, OverlapSphereRadius,
					TArray<TEnumAsByte<EObjectTypeQuery>> { UEngineTypes::ConvertToObjectType(ECC_Pawn) },
					AUpPlayableNpc::StaticClass(), TArray<AActor*> {}, OverlapActors);

				if (bDebugOverlapSphere)
				{
					UKismetSystemLibrary::DrawDebugSphere(this, SpherePosition,
						OverlapSphereRadius, 12, FColor::Orange, 5.f);
				}

				for (uint8 i = 0; PowerWheelButtons.IsValidIndex(i); i++)
				{
					if (const auto Button = PowerWheelButtons[i])
					{
						if (SquadMemberTags.IsValidIndex(i))
						{
							const auto SquadMemberTag = SquadMemberTags[i];
							AUpPlayableNpc* Npc = nullptr;

							for (const auto OverlapActor : OverlapActors)
							{
								if (const auto OverlapNpc = Cast<AUpPlayableNpc>(OverlapActor))
								{
									if (const auto TagIdable = Cast<IUpTagIdable>(OverlapNpc))
									{
										if (TagIdable->GetTagId().MatchesTagExact(SquadMemberTag))
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
								Button->SetNpcTag(SquadMemberTag);
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
