// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MultiplayerSessionSetting.h"

#include "Gameplay/C_LobbyGameMode.h"

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

FText UC_MultiplayerSessionSetting::GetSectionText() const
{
	return NSLOCTEXT("MultiplayersSessionSettings", "SectionName", "多人会话");
}

FText UC_MultiplayerSessionSetting::GetSectionDescription() const
{
	return NSLOCTEXT("MultiplayersSessionSettings", "SectionDescription", "多人会话配置");
}
