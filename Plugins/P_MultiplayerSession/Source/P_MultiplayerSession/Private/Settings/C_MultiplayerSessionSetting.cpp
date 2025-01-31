// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MultiplayerSessionSetting.h"

#include "Gameplay/C_LobbyGameMode.h"
FString UC_MultiplayerSessionSetting::UnassignedTeamName = TEXT("未分配");

UC_MultiplayerSessionSetting::UC_MultiplayerSessionSetting(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	CategoryName = TEXT("Plugins");
	SectionName = "Multiplayer";
	LobbyMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/P_MultiplayerSession/Map/DefaultLobbyMap.DefaultLobbyMap")));
	TransitionMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/P_MultiplayerSession/Map/DefaultTransitionMap.DefaultTransitionMap")));
	TransitionGameMode = AC_LobbyGameMode::StaticClass();
}

UC_MultiplayerSessionSetting* UC_MultiplayerSessionSetting::Get()
{
	UC_MultiplayerSessionSetting* Settings = GetMutableDefault<UC_MultiplayerSessionSetting>();
	check(Settings);
	return Settings;
}

FName UC_MultiplayerSessionSetting::GetSessionName() const
{
	FName ResultSessionName;
	if (bShareMainArea)
	{
		ResultSessionName = FName(FString::Printf(TEXT("%s_%s"), *MainAreaName, *SeatName));
	}
	else
	{
		ResultSessionName = FName(FString::Printf(TEXT("%s_%s_%s"), *MainAreaName, *MinorAreaName, *SeatName));
	}

	return ResultSessionName;
}

#if WITH_EDITOR

FText UC_MultiplayerSessionSetting::GetSectionText() const
{
	return NSLOCTEXT("MultiplayerSessionSettings", "SectionName", "多人会话");
}

FText UC_MultiplayerSessionSetting::GetSectionDescription() const
{
	return NSLOCTEXT("MultiplayerSessionSettings", "SectionDescription", "多人会话配置");
}

void UC_MultiplayerSessionSetting::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// FName PropertyName = PropertyChangedEvent.GetPropertyName();
	// if (PropertyName != GET_MEMBER_NAME_CHECKED(UC_MultiplayerSessionSetting, Teams) && PropertyName != GET_MEMBER_NAME_CHECKED(UC_MultiplayerSessionSetting, MaxPlayers))
	// 	return;
	//
	// int SumTeamMember = 0;
	// for (const auto &Team : Teams)
	// {
	// 	SumTeamMember += Team.MaxPlayerNum;
	// }
	//
	// if (SumTeamMember > MaxPlayers)
	// {
	// 	const uint8 AverageMember = MaxPlayers / Teams.Num();
	// 	uint8 RemainingMember = MaxPlayers - AverageMember * Teams.Num();
	// 	for (auto& Team : Teams)
	// 	{
	// 		Team.MaxPlayerNum = AverageMember;
	// 		
	// 		if (RemainingMember > 0)
	// 		{
	// 			Team.MaxPlayerNum += 1;
	// 			RemainingMember -= 1;
	// 		}
	// 	}
	// }
}

#endif
