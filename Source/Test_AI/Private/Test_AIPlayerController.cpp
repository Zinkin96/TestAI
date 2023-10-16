// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test_AIPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Test_AICharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Test_GeneralPawnInterface.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BehaviorTree/BehaviorTree.h"

ATest_AIPlayerController::ATest_AIPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ATest_AIPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	 bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, true, Hit);
	 if (bHitSuccessful)
	 {
		 if (Cast<APawn>(Hit.GetActor()))
		 {
			 ITest_GeneralPawnInterface::Execute_SetTarget(GetPawn(), Hit.GetActor());
		 }
		 else
		 {
			 ITest_GeneralPawnInterface::Execute_SetTarget(GetPawn(), NULL);
		 }
	 }
	 else
	 {
		 ITest_GeneralPawnInterface::Execute_SetTarget(GetPawn(), NULL);
	 }
}

void ATest_AIPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ATest_AIPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATest_AIPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATest_AIPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATest_AIPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATest_AIPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ATest_AIPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ATest_AIPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ATest_AIPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ATest_AIPlayerController::OnTouchReleased);

		// Setup ability input events
		EnhancedInputComponent->BindAction(SetUseAbiliy1Action, ETriggerEvent::Started, this, &ATest_AIPlayerController::OnUseAbiliy1Started);
		EnhancedInputComponent->BindAction(SetUseAbiliy2Action, ETriggerEvent::Started, this, &ATest_AIPlayerController::OnUseAbiliy2Started);
	}
}

void ATest_AIPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ATest_AIPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
		if (IsValid(AbilitySystemComp))
		{
			AbilitySystemComp->CancelAllAbilities();
		}
	}
}

void ATest_AIPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ATest_AIPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ATest_AIPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ATest_AIPlayerController::OnUseAbiliy1Started()
{
	UseAbility(SetUseAbiliy1Action);
}

void ATest_AIPlayerController::OnUseAbiliy2Started()
{
	UseAbility(SetUseAbiliy2Action);
}

void ATest_AIPlayerController::UseAbility(UInputAction*& InputAction)
{
	if (!IsValid(AbilitySystemComp))
	{
		return;
	}

	FGameplayTag* AbilityTag = AbilityInput.Find(InputAction);
	FGameplayTagContainer qwe(*AbilityTag);
	AbilitySystemComp->TryActivateAbilitiesByTag(qwe, true);
	return;
}

void ATest_AIPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
}
