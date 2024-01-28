// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/UpGameplayAbility.h"
#include "UpSwitchOutOfSquadMemberAbility.generated.h"

class USoundCue;

UCLASS()
class UNREALPORTFOLIO_API UUpSwitchOutOfSquadMemberAbility : public UUpGameplayAbility
{
	GENERATED_BODY()

public:
	UUpSwitchOutOfSquadMemberAbility();

	virtual void OnGameplayTaskInitialized(UGameplayTask& Task) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TObjectPtr<USoundCue> Sfx_MergeOut;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	FLinearColor CameraFadeColor = FLinearColor::Black;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float CameraFadeEndAlpha = 0.5f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float CameraFadeDuration = 0.2f;
};
