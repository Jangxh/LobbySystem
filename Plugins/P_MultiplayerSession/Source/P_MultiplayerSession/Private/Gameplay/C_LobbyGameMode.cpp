// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyGameMode.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Gameplay/C_LobbyPlayerController.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Library/C_MultiplayerSessionUtil.h"
#include "Settings/C_MultiplayerSessionSetting.h"

AC_LobbyGameMode::AC_LobbyGameMode(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PlayerControllerClass = AC_LobbyPlayerController::StaticClass();

	const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(UObject::GetWorld());
	if (OnlineSubsystem == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("OnlineSubsystem is null"));
		return;
	}

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (SessionInterface == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("SessionInterface is null"));
		return;
	}
}

void AC_LobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APlayerController* PlayerController = Cast<APlayerController>(NewPlayer);
	if (PlayerController == nullptr)
		return;
	
	if (SessionInterface == nullptr || !SessionInterface.IsValid())
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("SessionInterface is null"));
		return;
	}

	UC_MultiplayerSessionSetting* SessionSetting = UC_MultiplayerSessionSetting::Get();
	if (SessionSetting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Session setting is null"));
		return;
	}
	
	const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
	FName SessionName = SessionSetting->GetSessionName();
	const FNamedOnlineSession* ExistingSession = SessionPtr->GetNamedSession(SessionName);
	if (ExistingSession == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("session[%s] not existed"), *SessionName.ToString());
		return;
	}

	FOnlineSessionSettings* CurSessionSettings = SessionPtr->GetSessionSettings(SessionName);
	if (CurSessionSettings == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("session[%s] setting not found"), *SessionName.ToString());
		return;
	}

	if (CurSessionSettings->NumPublicConnections <= 0)
	{
		UE_LOG(LogMultiplayerSession, Warning, TEXT("now session is full"));
		const FSoftObjectPath LobbyMapPath = SessionSetting->LobbyMap.ToSoftObjectPath();
		if (!SessionSetting->LobbyMap.IsValid() && !LobbyMapPath.IsValid())
		{
			UE_LOG(LogMultiplayerSession, Error, TEXT("Lobby map is not valid"));
			return;
		}
		const FString LobbyMapPathStr = LobbyMapPath.GetLongPackageName();
		PlayerController->ClientTravel(LobbyMapPathStr, TRAVEL_Absolute);
		return;
	}
	
	CurSessionSettings->NumPublicConnections -= 1;
	const bool bSuccess = SessionPtr->UpdateSession(SessionName, *CurSessionSettings);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("session[%s] update session setting failed"), *SessionName.ToString());\
		return;
	}
}
