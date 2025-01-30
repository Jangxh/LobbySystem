// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/C_MultiplayerSessionSubsystem.h"

#include "C_MultiplayerSessionLogCategory.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Gameplay/C_LobbyGameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/C_MultiplayerSessionSetting.h"

UC_MultiplayerSessionSubsystem::UC_MultiplayerSessionSubsystem():
CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
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

	const UC_MultiplayerSessionSetting* MultiplayerSessionSetting = UC_MultiplayerSessionSetting::Get();
	if (MultiplayerSessionSetting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("MultiiplayersSessionSetting is null"));
		return;
	}

	PrivateSessionName = MultiplayerSessionSetting->GetSessionName();
}

UC_MultiplayerSessionSubsystem* UC_MultiplayerSessionSubsystem::Get()
{
	const UWorld *World = GWorld;
	if (World == nullptr)
		return nullptr;

	const UGameInstance* GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
		return nullptr;

	return GameInstance->GetSubsystem<UC_MultiplayerSessionSubsystem>();
}

void UC_MultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UC_MultiplayerSessionSubsystem::CreateSession(FUniqueNetIdPtr UserID)
{
	if (!SessionInterface.IsValid() || PrivateSessionName == NAME_None || !UserID.IsValid())
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("SessionInterface is null or session name is none"));
		OnMultiplayerSessionCreated.Broadcast(false);
		return false;
	}

	const UC_MultiplayerSessionSetting* MultiplayersSessionSetting = UC_MultiplayerSessionSetting::Get();
	if (MultiplayersSessionSetting == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("MultiplayerSessionSetting is null"));
		OnMultiplayerSessionCreated.Broadcast(false);
		return false;
	}

	const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
	const FNamedOnlineSession* ExistingSession = SessionPtr->GetNamedSession(PrivateSessionName);
	if (ExistingSession != nullptr)
	{
		SessionPtr->DestroySession(PrivateSessionName);
	}

	CreateSessionCompleteDelegateHandle = SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	TSharedPtr<FOnlineSessionSettings> OnlineSessionSettings = MakeShared<FOnlineSessionSettings>();
	OnlineSessionSettings->bIsLANMatch = true;
	OnlineSessionSettings->NumPublicConnections = MultiplayersSessionSetting->MaxPlayers;
	OnlineSessionSettings->bShouldAdvertise = true;
	OnlineSessionSettings->Set(FName("MatchType"), PrivateSessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	const bool bSuccess = SessionPtr->CreateSession(*UserID, PrivateSessionName, *OnlineSessionSettings);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Failed to create create session request"));
		SessionPtr->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnMultiplayerSessionCreated.Broadcast(false);
		return false;
	}
	
	return true;
}

bool UC_MultiplayerSessionSubsystem::FindSessions(int MaxSearchResult, FUniqueNetIdPtr UserID)
{
	if (!SessionInterface.IsValid() || !UserID.IsValid())
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("SessionInterface or userid is null"));
		OnMultiplayerSessionFound.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return false;
	}

	if (LastSessionSearch != nullptr && LastSessionSearch->SearchState == EOnlineAsyncTaskState::InProgress)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("search session is in process"));
		return false;
	}
	
	const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
	
	FindSessionsCompleteDelegateHandle = SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	
	LastSessionSearch = MakeShared<FOnlineSessionSearch>();
	LastSessionSearch->MaxSearchResults = MaxSearchResult;
	LastSessionSearch->bIsLanQuery = true;
	
	const bool bSuccess = SessionPtr->FindSessions(*UserID, LastSessionSearch.ToSharedRef());
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Failed to create find session request"));
		SessionPtr->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		OnMultiplayerSessionFound.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return false;
	}
	return true;
}

bool UC_MultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionSearchResult, FUniqueNetIdPtr UserID)
{
	if (!SessionInterface.IsValid() || !UserID.IsValid() || PrivateSessionName == NAME_None)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("SessionInterface or userid is null"));
		OnMultiplayerSessionJoined.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return false;
	}

	FString MatchType;
	bool bSuccess = SessionSearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Failed to get session match type"));
		OnMultiplayerSessionJoined.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return false;
	}
	
	const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
	JoinSessionCompleteDelegateHandle = SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	bSuccess = SessionPtr->JoinSession(*UserID, FName(MatchType), SessionSearchResult);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Failed to create join session request"));
		SessionPtr->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		OnMultiplayerSessionJoined.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return false;
	}

	return true;
}

bool UC_MultiplayerSessionSubsystem::StartSession(FName SessionName)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("SessionInterface is null"));
		OnMultiplayerSessionStarted.Broadcast(false);
		return false;
	}

	const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
	if (SessionName == NAME_None)
	{
		SessionName = PrivateSessionName;
	}

	const FNamedOnlineSession* ExistingSession = SessionPtr->GetNamedSession(SessionName);
	if (ExistingSession == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("sesssion[%s] not existed"), *SessionName.ToString());
		OnMultiplayerSessionStarted.Broadcast(false);
		return false;
	}
	
	StartSessionCompleteDelegateHandle = SessionPtr->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);
	const bool bSuccess = SessionPtr->StartSession(SessionName);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Failed to create start session reqeust"));
		OnMultiplayerSessionStarted.Broadcast(false);
		SessionPtr->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
		return false;
	}

	return true;
}                         

void UC_MultiplayerSessionSubsystem::FilterSearchResult() const
{
	if (LastSessionSearch == nullptr || LastSessionSearch->SearchState != EOnlineAsyncTaskState::Done)
	{
		return;
	}

	const UC_MultiplayerSessionSetting* MultiplayerSessionSetting = UC_MultiplayerSessionSetting::Get();
	if (MultiplayerSessionSetting == nullptr)
	{
		return;
	}
	
	TArray<FOnlineSessionSearchResult> MatchedResults;
	for (const FOnlineSessionSearchResult& SearchResult : LastSessionSearch->SearchResults)
	{
		FString MatchType;
		const bool bSuccess = SearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		if (!bSuccess)
			continue;
		
		// 首先判定主站区是否一致，如果不一致则移除
		if (!MatchType.StartsWith(MultiplayerSessionSetting->MainAreaName))
		{
			continue;
		}

		// 如果不共享主战区，则需要进一步判定子战区是否一致
		if (!MultiplayerSessionSetting->bShareMainArea)
		{
			TArray<FString> MatchInfos;
			MatchType.ParseIntoArray(MatchInfos, TEXT("_"), false);
			if (MatchInfos.Num() != 3)
			{
				continue;
			}

			if (MatchInfos[1] != MultiplayerSessionSetting->MinorAreaName)
			{
				continue;
			}
		}

		if (SearchResult.Session.SessionSettings.NumPublicConnections <= 0)
		{
			UE_LOG(LogMultiplayerSession, Display, TEXT("session[%s] is full"), *MatchType);
			continue;
		}
		
		MatchedResults.Add(SearchResult);
	}

	LastSessionSearch->SearchResults = MatchedResults;
}

void UC_MultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogMultiplayerSession, Display, TEXT("session[%s] created success: %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false"));
	if (SessionInterface.IsValid())
	{
		const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
		SessionPtr->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	
	OnMultiplayerSessionCreated.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{
		// 成功情况下进入中转大厅
		const UC_MultiplayerSessionSetting* Setting = UC_MultiplayerSessionSetting::Get();
		UWorld* World = GetWorld();
		if (Setting == nullptr || World == nullptr)
		{
			UE_LOG(LogMultiplayerSession, Warning, TEXT("cannot find setting or world when travelling"))
			return;
		}

		if (Setting->TransitionGameMode == nullptr)
		{
			UE_LOG(LogMultiplayerSession, Warning, TEXT("game mode not setted"))
			return;
		}
		
		const UPackage* Package = Setting->TransitionGameMode->GetPackage();
		const FString PackageName = Package->GetName();
		const FString GameModeObjectName = Setting->TransitionGameMode->GetName();
		const FString GameModeFullPath = FString::Printf(TEXT("%s.%s"), *PackageName, *GameModeObjectName);

		const FString Options = FString::Printf(TEXT("listen?game=%s"), *GameModeFullPath);
		UGameplayStatics::OpenLevelBySoftObjectPtr(World, Setting->TransitionMap, true, Options);
	}
}

void UC_MultiplayerSessionSubsystem::OnFindSessionComplete(bool bSuccess)
{
	if (SessionInterface.IsValid())
	{
		const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
		SessionPtr->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch == nullptr)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("Last Session Search Destroyed"));
		OnMultiplayerSessionFound.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	UE_LOG(LogMultiplayerSession, Display, TEXT("find session complete success: %s"), bSuccess ? TEXT("true") : TEXT("false"))
	FilterSearchResult();
	
	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		OnMultiplayerSessionFound.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	OnMultiplayerSessionFound.Broadcast(LastSessionSearch->SearchResults, bSuccess);
}

void UC_MultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogMultiplayerSession, Display, TEXT("join session[%s] result: %d"), *SessionName.ToString(), Result);

	if (SessionInterface.IsValid())
	{
		const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
		SessionPtr->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	OnMultiplayerSessionJoined.Broadcast(Result);
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		return;
	}

	const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
	FString Address;
	const bool bSuccess = SessionPtr->GetResolvedConnectString(SessionName, Address);
	if (!bSuccess)
	{
		UE_LOG(LogMultiplayerSession, Error, TEXT("get session[%s] connect string failed"), *SessionName.ToString())
		return;
	}

	UE_LOG(LogMultiplayerSession, Display, TEXT("travel to %s"), *Address);
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController != nullptr)
	{
		PlayerController->ClientTravel(Address, TRAVEL_Absolute);
	}
}

void UC_MultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogMultiplayerSession, Display, TEXT("start session[%s] success: %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false"));
	if (SessionInterface.IsValid())
	{
		const IOnlineSessionPtr SessionPtr = SessionInterface.Pin();
		SessionPtr->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	OnMultiplayerSessionStarted.Broadcast(bWasSuccessful);
}
