// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_HUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/TestCharacter_HPBar.h"



ATest_HUD::ATest_HUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetMainClassFinder(TEXT("/Game/UI/WBP_Main"));
	if (WidgetMainClassFinder.Succeeded())
	{
		WidgetMainClass = WidgetMainClassFinder.Class;
	}
}

void ATest_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetMainClass)
	{
		TObjectPtr<UUserWidget> MainWidgetRef= CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetMainClass);
		MainWidgetRef->AddToViewport();
	}
}