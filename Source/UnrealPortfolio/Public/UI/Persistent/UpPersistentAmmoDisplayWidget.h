// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/UpWeapon.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Structs.h"
#include "UpPersistentAmmoDisplayWidget.generated.h"

class AUpWeapon;
struct FGameplayTag;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentAmmoDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	float MagazineFill = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float MagazineCapacity = 0.f;
	UPROPERTY(BlueprintReadOnly)
	float AmmoReserve = 0.f;

	UPROPERTY(Transient)
	TObjectPtr<AUpWeapon> Weapon;
	UPROPERTY(BlueprintReadOnly)
	FUpItemData ItemData;
	
	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetRootVisibility() const;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnCustomHudSet_Implementation(AUpHud* NewCustomHud) override;

private:
	UFUNCTION()
	void HandleActiveWeaponChange(AUpWeapon* InWeapon);
	UFUNCTION()
	void HandleAttributeValueChange(const FGameplayTag& TagId, const FGameplayTag& AttributeTag, const float Value);

	void UpdateValues();
};
