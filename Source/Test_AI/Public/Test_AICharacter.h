// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Test_AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Test_GeneralPawnInterface.h"
#include "Delegates/Delegate.h"
#include "UI/TestCharacter_HPBar.h"
#include "Test_AICharacter.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, MaxHealth, float, CurrentHealth);

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

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	FORCEINLINE virtual void SetTarget_Implementation(AActor* Target) override { TargetActor = Target; }

	FORCEINLINE virtual AActor* GetTarget_Implementation() const override { return TargetActor; }

	FORCEINLINE virtual int32 GetHealthPotionsCount_Implementation() const override { return HealPotionsCount; }

	FORCEINLINE virtual void SetHealthPotionsCount_Implementation(int32 Count) override { HealPotionsCount = Count; }

	FORCEINLINE virtual float GetHealthPotionsHealthRestore_Implementation() const override { return HealPotionsHealthRestore; }

	UPROPERTY(BlueprintAssignable)
		FOnHealthChangedDelegate OnHealthChanged;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UTest_AttributeSet> AttributeSet = CreateDefaultSubobject<UTest_AttributeSet>(TEXT("AttributeSet"));

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UWidgetComponent> HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	void OnHealthChange(const FOnAttributeChangeData& Data);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "Melee attack damage", Category = "Character stats")
		float MeleeAttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Heal potion count", Category = "Character stats")
		int32 HealPotionsCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "Heal potions health restore", Category = "Character stats")
		float HealPotionsHealthRestore = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Character abilities", Category = "Ability system")
		TSet<TSubclassOf<UGameplayAbility>> CharacterAbilities;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	TObjectPtr<AActor> TargetActor;

};

