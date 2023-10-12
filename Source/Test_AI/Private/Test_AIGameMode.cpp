// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test_AIGameMode.h"
#include "Test_AIPlayerController.h"
#include "Test_AICharacter.h"
#include "UObject/ConstructorHelpers.h"

ATest_AIGameMode::ATest_AIGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATest_AIPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

