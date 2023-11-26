// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "UpNavLinkProxy.generated.h"

UENUM()
namespace EUpNavLinkProxyNavigationType {
	enum Type : uint8
	{
		Jump,
		Mantle
	};
}

UCLASS()
class UNREALPORTFOLIO_API AUpNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	AUpNavLinkProxy();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category="UP Params")
	float MovementDuration = 1.f;
	UPROPERTY(EditAnywhere, Category="UP Params")
	TEnumAsByte<EUpNavLinkProxyNavigationType::Type> NavigationType = EUpNavLinkProxyNavigationType::Jump;
	
	UFUNCTION()
	void HandleSmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint);
};
