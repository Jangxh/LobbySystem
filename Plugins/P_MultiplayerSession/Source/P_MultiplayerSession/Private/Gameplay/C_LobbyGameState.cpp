// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/C_LobbyGameState.h"


#include "C_MultiplayerSessionLogCategory.h"
#include "Net/UnrealNetwork.h"
#include "Settings/C_MultiplayerSessionSetting.h"

AC_LobbyGameState::AC_LobbyGameState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	UC_MultiplayerSessionSetting* Settings = UC_MultiplayerSessionSetting::Get();
	if (Settings == nullptr)
		return;

	for (const auto & Team : Settings->Teams)
	{
		FTeamStateInfo TeamInfo;
		TeamInfo.TeamName = Team.TeamName;
		TeamInfo.MaxPlayers = Team.MaxPlayerNum;
		TeamStateInfos.Add(TeamInfo);
	}

	FTeamStateInfo TeamInfo;
	TeamInfo.TeamName = Settings->UnassignedTeamName;
	TeamInfo.MaxPlayers = Settings->MaxPlayers;
	TeamStateInfos.Add(TeamInfo);
}

void AC_LobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AC_LobbyGameState, TeamStateInfos);
}

TArray<FString> AC_LobbyGameState::GetTeamMembers(const int& TeamIndex) const
{
	if (TeamStateInfos.Num() == 1)
		return TArray<FString>();
	
	if (TeamStateInfos.Num() <= TeamIndex)
		return TArray<FString>();

	int ActualTeamIndex = TeamIndex;
	if (TeamIndex == INDEX_NONE)
	{
		ActualTeamIndex = TeamStateInfos.Num() - 1;
	}
	return TeamStateInfos[ActualTeamIndex].TeamMembers;
}

bool AC_LobbyGameState::SwitchTeam(const int& CurTeamIndex, const int& NewTeamIndex, const FString& MemberName)
{
	if (TeamStateInfos.Num() == 1)
		return false;
	
	if (CurTeamIndex == NewTeamIndex)
		return true;
	
	int ActualCurTeamIndex = CurTeamIndex;
	if (ActualCurTeamIndex == INDEX_NONE)
	{
		ActualCurTeamIndex = TeamStateInfos.Num() - 1;
	}

	int ActualNewTeamIndex = NewTeamIndex;
	if (ActualNewTeamIndex == INDEX_NONE)
	{
		ActualNewTeamIndex = TeamStateInfos.Num() - 1;
	}

	if (TeamStateInfos[ActualNewTeamIndex].TeamMembers.Num() >= TeamStateInfos[ActualNewTeamIndex].MaxPlayers)
		return false;
	
	TeamStateInfos[ActualCurTeamIndex].TeamMembers.Remove(MemberName);
	TeamStateInfos[ActualNewTeamIndex].TeamMembers.Add(MemberName);
	return true;
}


bool AC_LobbyGameState::AssignToDefaultTeam(const FString& MemberName)
{
	if (TeamStateInfos.Num() == 1)
		return false;
	
	UC_MultiplayerSessionSetting* Settings = UC_MultiplayerSessionSetting::Get();
	if (Settings == nullptr)
		return false;

	FTeamStateInfo& DefaultTeamInfo = TeamStateInfos[Settings->Teams.Num()];
	if (DefaultTeamInfo.TeamMembers.Num() >= DefaultTeamInfo.MaxPlayers)
	{
		UE_LOG(LogMultiplayerSession, Warning, TEXT("team[%s] has full"), *DefaultTeamInfo.TeamName)
		return false;
	}

	DefaultTeamInfo.TeamMembers.Add(MemberName);
	return true;
}

void AC_LobbyGameState::RemoveFromTeam(const FString& MemberName)
{
	for (auto & TeamInfo : TeamStateInfos)
	{
		if (TeamInfo.TeamMembers.Contains(MemberName))
		{
			TeamInfo.TeamMembers.Remove(MemberName);
		}
	}
}
