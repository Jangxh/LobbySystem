// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/C_FindMultiplayerSessionsCallbackProxy.h"

#include "Settings/C_MultiplayerSessionSetting.h"
#include "SubSystem/C_MultiplayerSessionSubsystem.h"

void FBlueprintMultiplayerSessionResult::ConstructSessionName()
{
	UC_MultiplayerSessionSetting* Setting = UC_MultiplayerSessionSetting::Get();
	if (Setting == nullptr)
		return;

	FString MatchType;
	const bool bSuccess = MultiplayerSessionResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
	if (!bSuccess)
		return;

	SessionName = MatchType;
	TArray<FString> MatchInfos;
	MatchType.ParseIntoArray(MatchInfos, TEXT("_"), false);
	if (Setting->bShareMainArea)
	{
		if (MatchInfos.Num() == 2)
		{
			SessionDisplayName = MatchInfos[1];
			return;
		}
		return;
	}

	if (MatchInfos.Num() == 3)
	{
		SessionDisplayName = MatchInfos[2];
	}
}

UC_FindMultiplayerSessionsCallbackProxy::UC_FindMultiplayerSessionsCallbackProxy(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer),
	FindCompleteDelegate(FOnFindMultiplayerSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnFindMultiplayerSessionsComplete)),
	MaxResults(0)
{
	SessionSubsystem = UC_MultiplayerSessionSubsystem::Get();
}

UC_FindMultiplayerSessionsCallbackProxy* UC_FindMultiplayerSessionsCallbackProxy::FindMultiplayerSessions(APlayerController* PlayerController, int32 MaxResult)
{
	UC_FindMultiplayerSessionsCallbackProxy* Proxy = NewObject<UC_FindMultiplayerSessionsCallbackProxy>();
	Proxy->PlayerController = PlayerController;
	Proxy->MaxResults = MaxResult;

	return Proxy;
}

void UC_FindMultiplayerSessionsCallbackProxy::Activate()
{
	const FUniqueNetIdPtr UserID = QueryIDFromPlayerController(PlayerController.Get());
	if (!UserID.IsValid())
	{
		OnFailure.Broadcast(TArray<FBlueprintMultiplayerSessionResult>());
		return;
	}

	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast(TArray<FBlueprintMultiplayerSessionResult>());
		return;
	}

	FindCompleteDelegateHandle = SessionSubsystem->OnMultiplayerSessionFound.Add(FindCompleteDelegate);
	const bool bSuccess = SessionSubsystem->FindSessions(MaxResults, UserID);
	if (!bSuccess)
	{
		SessionSubsystem->OnMultiplayerSessionFound.Remove(FindCompleteDelegateHandle);
		OnFailure.Broadcast(TArray<FBlueprintMultiplayerSessionResult>());
	}
}

void UC_FindMultiplayerSessionsCallbackProxy::OnFindMultiplayerSessionsComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bSuccess) const
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast(TArray<FBlueprintMultiplayerSessionResult>());
		return;
	}

	SessionSubsystem->OnMultiplayerSessionFound.Remove(FindCompleteDelegateHandle);
	if (bSuccess)
	{
		TArray<FBlueprintMultiplayerSessionResult> MultiplayerSessionResults;
		for (const FOnlineSessionSearchResult& Result : SessionResults)
		{
			FBlueprintMultiplayerSessionResult MultiplayerSessionResult;
			MultiplayerSessionResult.MultiplayerSessionResult = Result;
			MultiplayerSessionResult.ConstructSessionName();
			MultiplayerSessionResults.Add(MultiplayerSessionResult);
		}

		OnSuccess.Broadcast(MultiplayerSessionResults);
	}
	else
	{
		OnFailure.Broadcast(TArray<FBlueprintMultiplayerSessionResult>());
	}
}
