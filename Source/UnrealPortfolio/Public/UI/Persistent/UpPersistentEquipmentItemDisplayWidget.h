// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/UpCommonUserWidget.h"
#include "Utils/Enums.h"
#include "UpPersistentEquipmentItemDisplayWidget.generated.h"

class UCommonActionWidget;

UCLASS()
class UNREALPORTFOLIO_API UUpPersistentEquipmentItemDisplayWidget : public UUpCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetActivated(const bool bInActivated) { bActivated = bInActivated; }
	void SetImage(UTexture2D* InImage) { Image = InImage; }
	void ResetImage() { Image = Image_Empty; }
	
	UFUNCTION(BlueprintImplementableEvent)
	UCommonActionWidget* GetCommonActionWidget() const;

	FORCEINLINE EUpEquipmentSlot::Type GetEquipmentSlot() const { return EquipmentSlot; }

protected:
	UPROPERTY(EditDefaultsOnly, Category="UP Assets")
	TObjectPtr<UTexture2D> Image_Empty;
	
	UPROPERTY(EditAnywhere, Category="UP Params")
	TEnumAsByte<EUpEquipmentSlot::Type> EquipmentSlot = EUpEquipmentSlot::Weapon1;
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FLinearColor ImageColor_Activated = FLinearColor(FVector(1.f, 1.f, 1.f));
	UPROPERTY(EditDefaultsOnly, Category="UP Params")
	FLinearColor ImageColor_Deactivated = FLinearColor(FVector(0.25f, 0.25f, 0.25f));
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;

	bool bActivated = false;

	UFUNCTION(BlueprintCallable)
	FLinearColor GetImageColor() const;
	UFUNCTION(BlueprintCallable)
	ESlateVisibility GetCommonActionWidgetVisibility() const;

	virtual void NativePreConstruct() override;
};
