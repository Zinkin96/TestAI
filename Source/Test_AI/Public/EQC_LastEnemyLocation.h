// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_LastEnemyLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEST_AI_API UEQC_LastEnemyLocation : public UEnvQueryContext
{
	GENERATED_BODY()
	
private:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
