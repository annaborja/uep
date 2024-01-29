// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/UpPlayableCharacter.h"
#include "UpTriggerBox.generated.h"

class UBoxComponent;

UENUM()
namespace EUpTriggerEffectType
{
	enum Type : uint8
	{
		NotifyLevel
	};
}

USTRUCT()
struct FUpTriggerEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EUpTriggerEffectType::Type> Type = EUpTriggerEffectType::NotifyLevel;
	UPROPERTY(EditAnywhere)
	bool bExecuteOnce = false;
	
	UPROPERTY(EditAnywhere)
	FUpScriptActorParams TriggererParams;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	
	bool bExecuted = false;
};


UCLASS()
class UNREALPORTFOLIO_API AUpTriggerBox : public AActor
{
	GENERATED_BODY()

public:
	AUpTriggerBox();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category="UP Params", meta=(TitleProperty="Type"))
	TArray<FUpTriggerEffect> TriggerEffects;
	
	bool NotifyLevel(const FUpTriggerEffect& TriggerEffect) const;
};
