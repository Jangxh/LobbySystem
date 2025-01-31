// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyPlayerController.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/C_MultiplayerSessionSetting.h"
#include "SubSystem/C_MultiplayerSessionSubsystem.h"


void AC_LobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AC_LobbyPlayerController::DestroyLocalOnlineSession_Implementation(FName SessionName)
{
	UC_MultiplayerSessionSubsystem* Subsystem = UC_MultiplayerSessionSubsystem::Get();
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->DestroySession(SessionName);
}

void AC_LobbyPlayerController::Server_LeaveRoom()
{
	const AGameStateBase* GameStateBase = UGameplayStatics::GetGameState(GetWorld());
	if (GameStateBase == nullptr)
	{
		return;
	}

	if (GameStateBase->PlayerArray.Num() > 1)
	{
		UE_LOG(LogMultiplayerSession, Display, TEXT("client is more than 1"));
		return;
	}

	UC_MultiplayerSessionSubsystem* SessionSubsystem = UC_MultiplayerSessionSubsystem::Get();
	if (SessionSubsystem == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Display, TEXT("session subsystem is null"));
		return;
	}

	// 销毁会话
	SessionSubsystem->DestroySession();

	TravelToLobbyMap();
}

void AC_LobbyPlayerController::Client_LeaveRoom()
{
	UC_MultiplayerSessionSetting* SessionSetting = UC_MultiplayerSessionSetting::Get();
	if (SessionSetting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Session setting is null"));
		return;
	}

	const FName SessionName = SessionSetting->GetSessionName();
	DestroyLocalOnlineSession(SessionName);

	TravelToLobbyMap();
}

void AC_LobbyPlayerController::TravelToLobbyMap()
{
	UC_MultiplayerSessionSetting* SessionSetting = UC_MultiplayerSessionSetting::Get();
	if (SessionSetting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Session setting is null"));
		return;
	}

	// 转移到初始地图
	UE_LOG(LogMultiplayerSession, Warning, TEXT("now session is full"));
	const FSoftObjectPath LobbyMapPath = SessionSetting->LobbyMap.ToSoftObjectPath();
	if (!SessionSetting->LobbyMap.IsValid() && !LobbyMapPath.IsValid())
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Lobby map is not valid"));
		return;
	}
	const FString LobbyMapPathStr = LobbyMapPath.GetLongPackageName();
	ClientTravel(LobbyMapPathStr, TRAVEL_Absolute);
}

// void AC_LobbyPlayerController::OnReplicated_Kicked()
// {
// 	if (!bIsKicked)
// 		return;
//
// 	UC_MultiplayerSessionSubsystem* Subsystem = UC_MultiplayerSessionSubsystem::Get();
// 	if (Subsystem == nullptr)
// 	{
// 		return;
// 	}
//
// 	Subsystem->DestroySession(SessionName);
// }

void AC_LobbyPlayerController::LeaveRoom_Implementation()
{
	if (IsLocalPlayerController() && HasAuthority())
	{
		Server_LeaveRoom();
	}
	else
	{
		Client_LeaveRoom();
	}
}
