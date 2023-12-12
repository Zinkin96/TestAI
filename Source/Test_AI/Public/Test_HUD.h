// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Test_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TEST_AI_API ATest_HUD : public AHUD
{
	GENERATED_BODY()
	
public:

	ATest_HUD();

	virtual void BeginPlay() override;

private:

	TSubclassOf<class UUserWidget> WidgetMainClass;
};
