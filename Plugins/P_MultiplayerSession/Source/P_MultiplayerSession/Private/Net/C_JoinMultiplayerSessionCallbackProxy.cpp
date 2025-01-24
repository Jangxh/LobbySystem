// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/C_JoinMultiplayerSessionCallbackProxy.h"

#include "Net/C_FindMultiplayerSessionsCallbackProxy.h"
#include "SubSystem/C_MultiplayerSessionSubsystem.h"

UC_JoinMultiplayerSessionCallbackProxy::UC_JoinMultiplayerSessionCallbackProxy(const FObjectInitializer& InObjectInitializer):
Super(InObjectInitializer),
JoinCompleteDelegate(FOnJoinMultiplayerSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinMultiplayerSessionComplete))
{
	SessionSubsystem = UC_MultiplayerSessionSubsystem::Get();
}

UC_JoinMultiplayerSessionCallbackProxy* UC_JoinMultiplayerSessionCallbackProxy::JoinMultiplayerSession(APlayerController* PlayerController, const FBlueprintMultiplayerSessionResult& SearchResult)
{
	UC_JoinMultiplayerSessionCallbackProxy* Proxy = NewObject<UC_JoinMultiplayerSessionCallbackProxy>();
	Proxy->PlayerController = PlayerController;
	Proxy->OnlineSearchResult = SearchResult.MultiplayerSessionResult;

	return Proxy;
}

void UC_JoinMultiplayerSessionCallbackProxy::Activate()
{
	const FUniqueNetIdPtr UserID = QueryIDFromPlayerController(PlayerController.Get());
	if (!UserID.IsValid() || !OnlineSearchResult.IsValid())
	{
		OnFailure.Broadcast();
		return;
	}

	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	JoinCompleteDelegateHandle = SessionSubsystem->OnMultiplayerSessionJoined.Add(JoinCompleteDelegate);
	const bool bSuccess = SessionSubsystem->JoinSession(OnlineSearchResult, UserID);
	if (!bSuccess)
	{
		SessionSubsystem->OnMultiplayerSessionJoined.Remove(JoinCompleteDelegateHandle);
		OnFailure.Broadcast();
	}
}

void UC_JoinMultiplayerSessionCallbackProxy::OnJoinMultiplayerSessionComplete(EOnJoinSessionCompleteResult::Type Result) const
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	SessionSubsystem->OnMultiplayerSessionJoined.Remove(JoinCompleteDelegateHandle);
	if (Result == EOnJoinSessionCompleteResult::Success || Result == EOnJoinSessionCompleteResult::AlreadyInSession)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
