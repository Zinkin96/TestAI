// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Test_AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Test_GeneralPawnInterface.h"
#include "Test_AICharacter.generated.h"

UCLASS(Blueprintable)
class ATest_AICharacter : public ACharacter, public IAbilitySystemInterface, public ITest_GeneralPawnInterface
{
	GENERATED_BODY()

public:
	ATest_AICharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void SetTarget_Implementation(AActor* Target) override;

	virtual AActor* GetTarget_Implementation() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTest_AttributeSet* AttributeSet = CreateDefaultSubobject<UTest_AttributeSet>(TEXT("AttributeSet"));

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		//UWidgetComponent* WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	void OnHealthChange(const FOnAttributeChangeData& Data);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, DisplayName="Melee attack damage", Category = "Character stats")
		float MeleeAttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Character abilities", Category = "Ability system")
		TSet<TSubclassOf<UGameplayAbility>> CharacterAbilities;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	AActor* TargetActor;
};

