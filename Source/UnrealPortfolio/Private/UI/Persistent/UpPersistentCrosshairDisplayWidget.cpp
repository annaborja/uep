// Copyright AB. All Rights Reserved.

#include "UI/Persistent/UpPersistentCrosshairDisplayWidget.h"

#include "Characters/UpPlayableCharacter.h"
#include "Components/Border.h"
#include "Components/UpCharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	UpdateCrosshair();
}

void UUpPersistentCrosshairDisplayWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (const auto Character = Cast<AUpPlayableCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		CrosshairSpread = FMath::Clamp(UKismetMathLibrary::FInterpTo_Constant(
			CrosshairSpread, Character->GetVelocity().Size() / 10.f, InDeltaTime, InterpSpeed_Spread),
			CrosshairSpread_Min, CrosshairSpread_Max);
		
		if (const auto CustomMovementComponent = Character->GetCustomMovementComponent())
		{
			CrosshairOpacity = UKismetMathLibrary::FInterpTo_Constant(
				CrosshairOpacity, CustomMovementComponent->IsSprinting() ? 0.f : 1.f, InDeltaTime, InterpSpeed_Opacity);
		}
	}
	
	UpdateCrosshair();
}

void UUpPersistentCrosshairDisplayWidget::UpdateCrosshair() const
{
	if (const auto Widget = GetTopCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(0.0, -CrosshairSpread));
		Widget->SetRenderOpacity(CrosshairOpacity);
	}
	
	if (const auto Widget = GetBottomCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(0.0, CrosshairSpread));
		Widget->SetRenderOpacity(CrosshairOpacity);
	}
	
	if (const auto Widget = GetLeftCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(-CrosshairSpread, 0.0));
		Widget->SetRenderOpacity(CrosshairOpacity);
	}
	
	if (const auto Widget = GetRightCrosshair())
	{
		Widget->SetRenderTranslation(FVector2D(CrosshairSpread, 0.0));
		Widget->SetRenderOpacity(CrosshairOpacity);
	}
}
