// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyPlayerController.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "Settings/C_MultiplayerSessionSetting.h"
#include "UI/C_SessionMenu.h"

void AC_LobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// const UC_MultiplayerSessionSetting* Settings = UC_MultiplayerSessionSetting::Get();
	// if (Settings == nullptr)
	// {
	// 	UE_LOG(LogMultiplayerSession, Display, TEXT("Multiplayer session setting is null"));
	// 	return;
	// }
	//
	// if (Settings->SesssionMenuClass == nullptr)
	// 	return;
	//
	// UC_SessionMenu* SessioinMenu = CreateWidget<UC_SessionMenu>(this, Settings->SesssionMenuClass);
	// if (SessioinMenu == nullptr)
	// {
	// 	UE_LOG(LogMultiplayerSession, Display, TEXT("Session create failed"));
	// 	return;
	// }
	//
	// SessioinMenu->AddToViewport();
	// FInputModeGameAndUI InputMode;
	// InputMode.SetWidgetToFocus(SessioinMenu->TakeWidget());
	// InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// SetInputMode(InputMode);
	// SetShowMouseCursor(true);
}
