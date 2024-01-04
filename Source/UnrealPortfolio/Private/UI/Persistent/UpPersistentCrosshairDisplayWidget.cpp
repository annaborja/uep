// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentCrosshairDisplayWidget.h"

#include "Components/Border.h"

void UUpPersistentCrosshairDisplayWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (const auto Widget = GetTopCrosshair())
	{
		Widget->SetDesiredSizeScale(FVector2D(CrosshairThickness, CrosshairLength));
	}
	
	if (const auto Widget = GetBottomCrosshair())
	{
		Widget->SetDesiredSizeScale(FVector2D(CrosshairThickness, CrosshairLength));
	}
	
	if (const auto Widget = GetLeftCrosshair())
	{
		Widget->SetDesiredSizeScale(FVector2D(CrosshairLength, CrosshairThickness));
	}
	
	if (const auto Widget = GetRightCrosshair())
	{
		Widget->SetDesiredSizeScale(FVector2D(CrosshairLength, CrosshairThickness));
	}

	UpdateCrosshairSpread();
}

void UUpPersistentCrosshairDisplayWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUpPersistentCrosshairDisplayWidget::UpdateCrosshairSpread() const
{
	if (const auto Widget = GetTopCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(0.0, CrosshairSpread * -1.0));
	}
	
	if (const auto Widget = GetBottomCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(0.0, CrosshairSpread));
	}
	
	if (const auto Widget = GetLeftCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(CrosshairSpread * -1.0, 0.0));
	}
	
	if (const auto Widget = GetRightCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(CrosshairSpread, 0.0));
	}
}
