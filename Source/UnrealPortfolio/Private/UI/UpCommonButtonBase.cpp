// Copyright AB. All Rights Reserved.

#include "UI/UpCommonButtonBase.h"

void UUpCommonButtonBase::SetCustomHud(AUpHud* InCustomHud)
{
	if (CustomHud == InCustomHud) return;
	
	CustomHud = InCustomHud;
	OnCustomHudSet(CustomHud);
}

void UUpCommonButtonBase::OnCustomHudSet_Implementation(AUpHud* NewCustomHud) {}
