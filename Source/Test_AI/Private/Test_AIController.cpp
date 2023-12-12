// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_AIController.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "Test_GeneralPawnInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif


ATest_AIController::ATest_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 3500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 360.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	Perception->SetDominantSense(SightConfig->GetSenseImplementation());
	Perception->ConfigureSense(*SightConfig);
	ControllerTeamId = FGenericTeamId(0);
}

void ATest_AIController::BeginPlay()
{
	Super::BeginPlay();

	UseBlackboard(BlackBoardAsset, BlackBoardComponent);
	RunBehaviorTree(BehaiviorTreeComponent);
}

void ATest_AIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	Perception->OnPerceptionUpdated.AddDynamic(this, &ATest_AIController::OnPerceptionUpdated_Delegate);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ATest_AIController::OnTargetPerceptionUpdated_Delegate);
	Perception->ProcessStimuli();
}

void ATest_AIController::OnPerceptionUpdated_Delegate(const TArray<AActor*>& UpdatedActors)
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, UpdatedActors[0]->GetFName().ToString());*/

}

void ATest_AIController::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	BlackBoardComponent->SetValueAsBool("bSeeEnemy", Stimulus.WasSuccessfullySensed());
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Stimulus.WasSuccessfullySensed()?TEXT("Successfully sensed = true"): TEXT("Successfully sensed = false"));*/
	if (Stimulus.WasSuccessfullySensed() && GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Actor->GetFName().ToString());*/
		SetEnemy(Actor);
	}
	else
	{
		BlackBoardComponent->SetValueAsVector("EnemyLastKnownLocation", Stimulus.StimulusLocation);
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 100.0f, 32, FColor::White, false, 5.0f, 0, 1.0f);
#endif
	}
}

void ATest_AIController::SetEnemy(AActor* Actor)
{
	const TObjectPtr<AActor> TargetActor = ITest_GeneralPawnInterface::Execute_GetTarget(GetPawn());
	if (TargetActor && (Actor != nullptr))
	{
		return;
	}
	BlackBoardComponent->SetValueAsObject("EnemyRef", Actor);
	ITest_GeneralPawnInterface::Execute_SetTarget(GetPawn(), Actor);
	TObjectPtr<UAbilitySystemComponent> TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (TargetASC)
	{
		OnTargetDeathDelegateHandler = TargetASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Dead"))).AddUObject(this, &ATest_AIController::OnTargetDeath);
	}
	return;
}

void ATest_AIController::OnTargetDeath(const FGameplayTag InTag, int32 NewCount)
{
	TObjectPtr<AActor> TargetActor = ITest_GeneralPawnInterface::Execute_GetTarget(GetPawn());
	if (!TargetActor)
	{
		return;
	}
	TObjectPtr<UAbilitySystemComponent> TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		TargetASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Dead"))).Remove(OnTargetDeathDelegateHandler);
	}
	SetEnemy(nullptr);
	Perception->ProcessStimuli();
}

FGenericTeamId ATest_AIController::GetGenericTeamId() const
{
	return ControllerTeamId;
}

ETeamAttitude::Type	ATest_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Cast<APawn>(&Other)->GetController()))
	{
		if (TeamAgent->GetGenericTeamId() = ControllerTeamId)
		{
			return ETeamAttitude::Friendly;
		}
	}
	if (Cast<APawn>(&Other))
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Neutral;
}
