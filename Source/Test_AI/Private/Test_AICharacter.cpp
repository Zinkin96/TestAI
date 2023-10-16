// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test_AICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"


ATest_AICharacter::ATest_AICharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);

	HPWidgetComponent->SetupAttachment(RootComponent);
	HPWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	static ConstructorHelpers::FClassFinder<UTestCharacter_HPBar> HPBarWidgetClass(TEXT("/Game/UI/WBP_CharacterState"));
	if (IsValid(HPBarWidgetClass.Class))
	{
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HPWidgetComponent->SetWidgetClass(HPBarWidgetClass.Class);
	}
}

void ATest_AICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATest_AICharacter::BeginPlay()
{
	Super::BeginPlay();

	//Give abilities to character
	for (int32 Index = 0; Index != CharacterAbilities.Num(); ++Index)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(CharacterAbilities.Array()[Index], 1, Index));
	}

	AttributeSet->SetMeleeDamage(MeleeAttackDamage);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute()).AddUObject(this, &ATest_AICharacter::OnHealthChange);

	//Set HP bar values
	//
	if (UTestCharacter_HPBar* HPBarWidget = Cast<UTestCharacter_HPBar>(HPWidgetComponent->GetWidget()))
	{
		OnHealthChanged.AddDynamic(HPBarWidget, &UTestCharacter_HPBar::SetHP);
		HPBarWidget->SetHP(AttributeSet->GetMaxHealth(), AttributeSet->GetCurrentHealth());
	}
}

void ATest_AICharacter::OnHealthChange(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(AttributeSet->GetMaxHealth(), AttributeSet->GetCurrentHealth());
}