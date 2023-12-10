// Copyright AB. All Rights Reserved.

#include "UI/Basic/UpInvisibleButtonWidget.h"

void UUpInvisibleButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIsFocusable(false);
}
