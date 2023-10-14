// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_UseAbility.generated.h"

/**
 *
 */
class UAbilitySystemComponent;
struct FAbilityEndedData;

UCLASS()
class TEST_AI_API UBTTask_UseAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UBehaviorTreeComponent* BTComponent;

	UAbilitySystemComponent* AbilitySystemComp;

	FDelegateHandle FinishDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag AbilityTag;

	void FinishUseAbility(const FAbilityEndedData& AbilityEndedData);
};
