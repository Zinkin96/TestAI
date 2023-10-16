// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "TestCharacter_HPBar.generated.h"

/**
 * 
 */
class ATest_AICharacter;

UCLASS()
class TEST_AI_API UTestCharacter_HPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HPBar;

	UFUNCTION(BlueprintCallable)
		void SetHP(float MaxHealth, float CurrentHealth);
};
