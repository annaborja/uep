// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/HUD.h"
#include "UpHud.generated.h"

class AUpPlayerController;
class UUpCommonActivatableWidget;
class UUpPersistentOverlayWidget;

USTRUCT(BlueprintType)
struct FUpMenuTabData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Label;
};

UCLASS()
class UNREALPORTFOLIO_API AUpHud : public AHUD
{
	GENERATED_BODY()

public:
	void Init(AUpPlayerController* InPlayerController);
	void OpenMainMenu() const;

	FORCEINLINE TSubclassOf<UUpCommonActivatableWidget> GetMenuSwitcherClass() const { return MenuSwitcherClass; }

private:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpCommonActivatableWidget> MenuSwitcherClass;
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TSubclassOf<UUpPersistentOverlayWidget> PersistentOverlayClass;
	
	UPROPERTY(Transient)
	TObjectPtr<AUpPlayerController> PlayerController;
	UPROPERTY(Transient)
	TObjectPtr<UUpPersistentOverlayWidget> PersistentOverlayWidget;
};
