// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_UseAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTFunctionLibrary.h"

EBTNodeResult::Type UBTTask_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BTComponent = &OwnerComp;

	AActor* Owner = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Owner)
	{
		return EBTNodeResult::Failed;
	}
	AbilitySystemComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if (!AbilitySystemComp)
	{
		return EBTNodeResult::Failed;
	}

	AbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
	FinishDelegate = AbilitySystemComp->OnAbilityEnded.AddUObject(this, &UBTTask_UseAbility::FinishUseAbility);
	return EBTNodeResult::InProgress;
}

void UBTTask_UseAbility::FinishUseAbility(const FAbilityEndedData& AbilityEndedData)
{
	AbilitySystemComp->OnAbilityEnded.Remove(FinishDelegate);
	FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
}
