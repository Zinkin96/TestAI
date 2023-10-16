// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Test_AIController.generated.h"

/**
 *
 */
UCLASS()
class TEST_AI_API ATest_AIController : public AAIController
{
	GENERATED_BODY()


public:

	ATest_AIController();

	virtual void BeginPlay() override;

private:

	TObjectPtr<UBlackboardData> BlackBoardAsset = LoadObject<UBlackboardData>(nullptr, TEXT("/Script/AIModule.BlackboardData'/Game/AI/Test_Blackboard.Test_Blackboard'"));

	TObjectPtr <UBehaviorTree> BehaiviorTreeComponent = LoadObject<UBehaviorTree>(nullptr, TEXT("/Script/AIModule.BehaviorTree'/Game/AI/Test_BehaviorTree.Test_BehaviorTree'"));

	UBlackboardComponent* BlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	UPROPERTY(VisibleAnywhere)
		UAIPerceptionComponent* Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_Perception"));

	UPROPERTY(VisibleAnywhere)
		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	UFUNCTION()
		virtual void OnPossess(APawn* PossessedPawn) override;

	UFUNCTION(BlueprintCallable)
		void OnPerceptionUpdated_Delegate(const TArray<AActor*>& UpdatedActors);

	UFUNCTION(BlueprintCallable)
		void OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus);

	virtual ETeamAttitude::Type	GetTeamAttitudeTowards(const AActor& Other) const override;

	FGenericTeamId ControllerTeamId;

	FGenericTeamId GetGenericTeamId() const;
};
