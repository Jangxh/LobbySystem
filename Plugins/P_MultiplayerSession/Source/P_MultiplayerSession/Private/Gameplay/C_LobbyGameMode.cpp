// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyGameMode.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Gameplay/C_LobbyPlayerController.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/GameSession.h"
#include "Interfaces/OnlineSessionInterface.h"
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
		
		// 先销毁本地会话，不然不能重新加入
		AC_LobbyPlayerController* LobbyPlayerController = Cast<AC_LobbyPlayerController>(PlayerController);
		if (LobbyPlayerController != nullptr)
		{
			LobbyPlayerController->bIsKicked = true;
			LobbyPlayerController->DestroyLocalOnlineSession(SessionSetting->GetSessionName());
		}

		const FSoftObjectPath LobbyMapPath = SessionSetting->LobbyMap.ToSoftObjectPath();
		if (!SessionSetting->LobbyMap.IsValid() && !LobbyMapPath.IsValid())
		{
			UE_LOG(LogMultiplayerSession, Error, TEXT("Lobby map is not valid"));
			return;
		}
		const FString LobbyMapPathStr = LobbyMapPath.GetLongPackageName();
		PlayerController->ClientTravel(LobbyMapPathStr, TRAVEL_Absolute);
		// GameSession->KickPlayer(PlayerController, FText::FromString(TEXT("session is full")));

		return;
	}
	
	CurSessionSettings->NumPublicConnections -= 1;
	const bool bSuccess = SessionPtr->UpdateSession(SessionName, *CurSessionSettings);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("session[%s] update session setting failed"), *SessionName.ToString());\
	}
	UE_LOG(LogMultiplayerSession, Display, TEXT("Post login now remain client: %d"), CurSessionSettings->NumPublicConnections);
}

void AC_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	APlayerController* PlayerController = Cast<APlayerController>(Exiting);
	if (PlayerController == nullptr)
		return;

	AC_LobbyPlayerController* LobbyPlayerController = Cast<AC_LobbyPlayerController>(PlayerController);
	if (LobbyPlayerController != nullptr && LobbyPlayerController->bIsKicked)
		return;
	
	if (PlayerController->IsLocalPlayerController() && PlayerController->HasAuthority())
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

	CurSessionSettings->NumPublicConnections += 1;
	const bool bSuccess = SessionPtr->UpdateSession(SessionName, *CurSessionSettings);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("session[%s] update session setting failed"), *SessionName.ToString());\
	}
	UE_LOG(LogMultiplayerSession, Display, TEXT("logout now remain client: %d"), CurSessionSettings->NumPublicConnections);
}
