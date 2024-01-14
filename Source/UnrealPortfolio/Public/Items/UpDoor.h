// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpItem.h"
#include "UpDoor.generated.h"

class USoundCue;

UCLASS()
class UNREALPORTFOLIO_API AUpDoor : public AUpItem
{
	GENERATED_BODY()

public:
	AUpDoor();
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void Interact(AUpPlayerController* PlayerController) override;

protected:
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TObjectPtr<USoundCue> Sfx_Close;
	UPROPERTY(EditAnywhere, Category="UP Assets")
	TObjectPtr<USoundCue> Sfx_Swing;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	FRotator Rotation_Closed = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category="UP Params")
	FRotator Rotation_Open = FRotator(0.f, -120.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	float RotationTolerance_Closed = 1.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float SwingSpeed = 5.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	float VolumeMultiplier_Closed = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="UP Runtime")
	bool bOpen = false;
	
	virtual void BeginPlay() override;

	virtual FText GetInteractionPromptText(const AUpPlayerController* PlayerController) const override;

private:
	bool bCloseSoundPlayed = false;
};
