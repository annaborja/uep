// Copyright AB. All Rights Reserved.

#include "Scripting/UpTriggerBox.h"

#include "Components/BoxComponent.h"
#include "Levels/UpLevelScriptActor.h"
#include "Utils/UpBlueprintFunctionLibrary.h"

AUpTriggerBox::AUpTriggerBox()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetBoxExtent(FVector(68.f, 136.f, 88.f));
	BoxComponent->SetMobility(EComponentMobility::Static);
	BoxComponent->SetCanEverAffectNavigation(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	for (const auto Channel : TArray<ECollisionChannel> { ECC_WorldDynamic, ECC_Pawn, ECC_Vehicle })
	{
		BoxComponent->SetCollisionResponseToChannel(Channel, ECR_Overlap);
	}
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AUpTriggerBox::OnBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& TriggerEffect : TriggerEffects)
	{
		if (TriggerEffect.bExecuteOnce && TriggerEffect.bExecuted) continue;
		if (!UUpBlueprintFunctionLibrary::SatisfiesActorParams(TriggerEffect.TriggererParams, OtherActor)) continue;
		
		switch (TriggerEffect.Type)
		{
		case EUpTriggerEffectType::NotifyLevel:
			TriggerEffect.bExecuted = NotifyLevel(TriggerEffect);
			break;
		default:
			break;
		}
	}
}

bool AUpTriggerBox::NotifyLevel(const FUpTriggerEffect& TriggerEffect) const
{
	if (TriggerEffect.Tag.IsValid())
	{
		if (const auto LevelScriptActor = UUpBlueprintFunctionLibrary::GetLevelScriptActor(this))
		{
			LevelScriptActor->NotifyTag(TriggerEffect.Tag);
			return true;
		}
	}

	return false;
}
