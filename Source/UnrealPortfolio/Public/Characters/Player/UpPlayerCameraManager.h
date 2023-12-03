// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "UpPlayerCameraManager.generated.h"

class AUpPlayableCharacter;

UCLASS()
class UNREALPORTFOLIO_API AUpPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AUpPlayerCameraManager();
	
protected:
	virtual void BeginPlay() override;
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	float CrouchBlendDuration = 0.5f;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayableCharacter> PlayableCharacter;
	
	float CrouchBlendTime;

	void AdjustCameraForCrouch(FTViewTarget& OutVT, const float DeltaTime);
};
