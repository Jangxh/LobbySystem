// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/C_MultiplayerSessionUtil.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/C_MultiplayerSessionSetting.h"

bool UC_MultiplayerSessionUtil::StartOnlineMatchSystem(const UObject* WorldContextObject)
{
	const UC_MultiplayerSessionSetting* Setting = UC_MultiplayerSessionSetting::Get();
	if (Setting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("multiplayer session setting is null"))
		return false;
	}

	FSoftObjectPath LobbyMapPath = Setting->LobbyMap.ToSoftObjectPath();
	if (!Setting->LobbyMap.IsValid() && !LobbyMapPath.IsValid())
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("lobby map is not valid"));
		return false;
	}

	UWorld* CurWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (CurWorld == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Current world is null"));
		return false;
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(CurWorld, Setting->LobbyMap, true);
	return true;
}


