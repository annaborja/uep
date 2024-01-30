// Copyright AB. All Rights Reserved.

#include "Scripting/UpTriggerVolume.h"

#include "AI/UpAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/UpCharacter.h"
#include "Components/ShapeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Levels/UpLevelScriptActor.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpTriggerVolume::AUpTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUpTriggerVolume::SetUpTriggerComponent(UShapeComponent* Component) const
{
	Component->SetupAttachment(GetRootComponent());
	Component->SetMobility(EComponentMobility::Static);
	Component->SetCanEverAffectNavigation(false);
	Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Component->SetCollisionResponseToAllChannels(ECR_Ignore);

	for (const auto Channel : TArray<ECollisionChannel> { ECC_WorldDynamic, ECC_Pawn, ECC_Vehicle })
	{
		Component->SetCollisionResponseToChannel(Channel, ECR_Overlap);
	}
	
	Component->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AUpTriggerVolume::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& TriggerEffect : TriggerEffects)
	{
		if (TriggerEffect.bExecuteOnce && TriggerEffect.bExecuted) continue;
		if (!UUpBlueprintFunctionLibrary::SatisfiesActorParams(TriggerEffect.TriggererParams, OtherActor)) continue;
		
		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Trigger effect %d on %s"), TriggerEffect.Type, *OtherActor->GetName())
		}
		
		switch (TriggerEffect.Type)
		{
		case EUpTriggerEffectType::NotifyLevel:
			TriggerEffect.bExecuted = NotifyLevel(TriggerEffect);
			break;
		case EUpTriggerEffectType::SetBlackboardKey:
			TriggerEffect.bExecuted = SetBlackboardKey(OtherActor, TriggerEffect);
			break;
		default:
			break;
		}
	}
}

bool AUpTriggerVolume::NotifyLevel(const FUpTriggerEffect& TriggerEffect) const
{
	if (TriggerEffect.Tag.IsValid())
	{
		if (const auto LevelScriptActor = UUpBlueprintFunctionLibrary::GetLevelScriptActor(this))
		{
			LevelScriptActor->NotifyTag(TriggerEffect.Tag);
			return true;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("Notify level failed: %s"), *TriggerEffect.Tag.ToString())
	
	return false;
}

bool AUpTriggerVolume::SetBlackboardKey(const AActor* Actor, const FUpTriggerEffect& TriggerEffect) const
{
	const auto Targets = TriggerEffect.ActorParams.ActorClass ? UUpBlueprintFunctionLibrary::FindActors(this, TriggerEffect.ActorParams.ActorClass, TriggerEffect.ActorParams.TagId)
		.FilterByPredicate([&TriggerEffect](const AActor* Target)
		{
			return UUpBlueprintFunctionLibrary::SatisfiesActorParams(TriggerEffect.ActorParams, Target);
		}) : TArray<AActor*> { nullptr };

	if (const auto BlackboardKey = UUpBlueprintFunctionLibrary::GetBlackboardKeyFromTag(TriggerEffect.Tag); BlackboardKey.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid blackboard key: %s"), *TriggerEffect.Tag.ToString())
	} else if (Targets.Num() != 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Unexpected number of targets found: %s, %d"), *TriggerEffect.ActorParams.TagId.ToString(), Targets.Num())
	} else
	{
		if (const auto Pawn = Cast<AUpCharacter>(Actor))
		{
			if (const auto PawnController = Cast<AUpAiController>(Pawn->GetController()))
			{
				if (const auto BlackboardComponent = PawnController->GetBlackboardComponent())
				{
					for (const auto Target : Targets)
					{
						if (Target)
						{
							if (BlackboardKey.ToString().EndsWith(TEXT("Location")))
							{
								auto Value = Target->GetActorLocation();

								// Allow for randomizing the target location.
								if (TriggerEffect.RelevantFloat > 0.f)
								{
									Value += UKismetMathLibrary::RandomUnitVector().GetSafeNormal2D() * TriggerEffect.RelevantFloat;
								}
							
								BlackboardComponent->SetValueAsVector(BlackboardKey, Value);
							} else
							{
								BlackboardComponent->SetValueAsObject(BlackboardKey, Target);
							}
						} else
						{
							BlackboardComponent->ClearValue(BlackboardKey);
						}
					}

					return true;
				}
			}
		}
		
		UE_LOG(LogTemp, Error, TEXT("Set blackboard key failed: %s / %s / %s"),
			*TriggerEffect.Tag.ToString(), *Actor->GetName(), *TriggerEffect.ActorParams.TagId.ToString())
	}

	return false;
}
