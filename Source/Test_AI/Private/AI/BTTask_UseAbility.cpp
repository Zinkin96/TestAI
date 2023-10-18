// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_UseAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Test_AIControllerInterface.h"

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

	//TMap<int32, FGameplayTag> AbilityOrder = ITest_AIControllerInterface::Execute_GetAbilityPriority(OwnerComp.GetAIOwner());
	//AbilityOrder.KeySort([](int32 A, int32 B) {	return A > B; });
	//for (TPair<int32, FGameplayTag> Ability : AbilityOrder)
	//{
	//	if (AbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(Ability.Value)))
	//	{
	//		FinishDelegate = AbilitySystemComp->OnAbilityEnded.AddUObject(this, &UBTTask_UseAbility::FinishUseAbility);
	//		return EBTNodeResult::InProgress;
	//	}		
	//}
	//return EBTNodeResult::Failed;
}

void UBTTask_UseAbility::FinishUseAbility(const FAbilityEndedData& AbilityEndedData)
{
	AbilitySystemComp->OnAbilityEnded.Remove(FinishDelegate);
	FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
}
