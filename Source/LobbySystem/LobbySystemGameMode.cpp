// Copyright Epic Games, Inc. All Rights Reserved.

#include "LobbySystemGameMode.h"
#include "LobbySystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALobbySystemGameMode::ALobbySystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
