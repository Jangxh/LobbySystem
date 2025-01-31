// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyPlayerController.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/C_LobbyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Settings/C_MultiplayerSessionSetting.h"
#include "SubSystem/C_MultiplayerSessionSubsystem.h"


void AC_LobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UC_MultiplayerSessionSetting* Settings = UC_MultiplayerSessionSetting::Get();
	if (Settings == nullptr)
		return;
	
	if (IsLocalPlayerController() && HasAuthority())
	{
		SetSeatName(Settings->SeatName);
	}
	else if (!HasAuthority())
	{
		SetSeatName(Settings->SeatName);
	}
}

void AC_LobbyPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AC_LobbyPlayerController, TeamIndex);
	DOREPLIFETIME(AC_LobbyPlayerController, SeatName);
}

void AC_LobbyPlayerController::SetSeatName_Implementation(const FString& InSeatName)
{
	SeatName = InSeatName;
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

void AC_LobbyPlayerController::SwitchTeam_Implementation()
{
	const UC_MultiplayerSessionSetting* Setting = UC_MultiplayerSessionSetting::Get();
	if (Setting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Warning, TEXT("multiplayer session setting is null"));
		return;
	}

	if (Setting->Teams.Num() == 0)
	{
		UE_LOG(LogMultiplayerSession, Warning, TEXT("team info is empty"));
		return;
	}

	AC_LobbyGameState* LobbyGameState = Cast<AC_LobbyGameState>(UGameplayStatics::GetGameState(this));
	if (LobbyGameState == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Warning, TEXT("multiplayer state is null"));
		return;
	}

	int NewTeamIndex;
	if (TeamIndex == INDEX_NONE)
	{
		NewTeamIndex = 0;
	}
	else if (TeamIndex == Setting->Teams.Num() - 1)
	{
		NewTeamIndex = INDEX_NONE;
	}
	else
	{
		NewTeamIndex = TeamIndex + 1;
	}
	
	if (LobbyGameState->SwitchTeam(TeamIndex, NewTeamIndex, SeatName))
	{
		TeamIndex = NewTeamIndex;
	}
}

FString AC_LobbyPlayerController::GetTeamName() const
{
	if (TeamIndex == INDEX_NONE)
		return UC_MultiplayerSessionSetting::UnassignedTeamName;
	
	UC_MultiplayerSessionSetting* Setting = UC_MultiplayerSessionSetting::Get();
	if (Setting == nullptr)
		return Setting->UnassignedTeamName;

	if (TeamIndex >= Setting->Teams.Num())
		return Setting->UnassignedTeamName;

	return Setting->Teams[TeamIndex].TeamName;
	
}

TArray<FString> AC_LobbyPlayerController::GetTeamMembers() const
{
	AC_LobbyGameState* LobbyGameState = Cast<AC_LobbyGameState>(UGameplayStatics::GetGameState(this));
	if (LobbyGameState == nullptr)
		return TArray<FString>();

	return LobbyGameState->GetTeamMembers(TeamIndex);
}

FString AC_LobbyPlayerController::GetSeatName() const
{
	return SeatName;
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
