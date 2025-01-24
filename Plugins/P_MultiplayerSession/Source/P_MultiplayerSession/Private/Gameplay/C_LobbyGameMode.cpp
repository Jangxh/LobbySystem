// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyGameMode.h"

#include "Gameplay/C_LobbyPlayerController.h"

AC_LobbyGameMode::AC_LobbyGameMode(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PlayerControllerClass = AC_LobbyPlayerController::StaticClass();
}
