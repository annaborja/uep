// Copyright AB. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "UpEqsContext_FactionCharacters.generated.h"

UCLASS()
class UNREALPORTFOLIO_API UUpEqsContext_FactionCharacters : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
