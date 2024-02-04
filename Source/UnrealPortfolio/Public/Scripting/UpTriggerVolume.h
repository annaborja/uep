// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UpTagIdable.h"
#include "Utils/Structs.h"
#include "UpTriggerVolume.generated.h"

UENUM()
namespace EUpTriggerEffectType
{
	enum Type : uint8
	{
		NotifyLevel,
		SetBlackboardKey
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
	FUpScriptActorParams ActorParams;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag Tag;
	UPROPERTY(EditAnywhere)
	float RelevantFloat = 0.f;
	
	bool bExecuted = false;
};

UCLASS()
class UNREALPORTFOLIO_API AUpTriggerVolume : public AActor, public IUpTagIdable
{
	GENERATED_BODY()
	
public:	
	AUpTriggerVolume();

	virtual FGameplayTag GetTagId() const override { return TagId; }
	
	FORCEINLINE uint8 GetTargetNumber() const { return TargetNumber; }

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetUpTriggerComponent(UShapeComponent* Component) const;

private:
	UPROPERTY(EditAnywhere, Category="UP Debug")
	bool bDebug = false;

	UPROPERTY(EditAnywhere, Category="UP Params")
	FGameplayTag TagId;
	UPROPERTY(EditAnywhere, Category="UP Params")
	uint8 TargetNumber = 1;
	
	UPROPERTY(EditAnywhere, Category="UP Params", meta=(TitleProperty="Type"))
	TArray<FUpTriggerEffect> TriggerEffects;
	
	bool NotifyLevel(const FUpTriggerEffect& TriggerEffect) const;
	bool SetBlackboardKey(const AActor* Actor, const FUpTriggerEffect& TriggerEffect) const;
};
