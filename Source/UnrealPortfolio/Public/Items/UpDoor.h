// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UpItem.h"
#include "UpDoor.generated.h"

class USoundCue;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpDoorOpenStateSignature, const bool);

UCLASS()
class UNREALPORTFOLIO_API AUpDoor : public AUpItem
{
	GENERATED_BODY()

public:
	FUpDoorOpenStateSignature OpenStateDelegate;
	
	AUpDoor();
	
	virtual void Tick(float DeltaSeconds) override;

	virtual bool Interact(AController* Controller) override;

	bool IsOpen() const { return bOpen; }
	void Open();

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

	virtual FText GetInteractionPromptText(const AController* Controller) const override;

private:
	bool bCloseSoundPlayed = false;

	void ToggleOpen();
};
