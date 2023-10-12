// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_LastEnemyLocation.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

void UEQC_LastEnemyLocation::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* QueryActor = Cast<AActor>(QueryInstance.Owner.Get());
	if (QueryActor == nullptr)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent =  UAIBlueprintHelperLibrary::GetBlackboard(QueryActor);
	if (BlackboardComponent == nullptr)
	{
		return;
	}

	UEnvQueryItemType_Point::SetContextHelper(ContextData, BlackboardComponent->GetValueAsVector("EnemyLastKnownLocation"));
}