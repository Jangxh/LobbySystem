// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/C_CreateMultiplayerSessionCallbackProxy.h"

#include "SubSystem/C_MultiplayerSessionSubsystem.h"

UC_CreateMultiplayerSessionCallbackProxy::UC_CreateMultiplayerSessionCallbackProxy(
	const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
	, CreateCompleteDelegate(FOnCreateMultiplayerSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateMultiplayerSessionComplete))
{
	SessionSubsystem = UC_MultiplayerSessionSubsystem::Get();
}

UC_CreateMultiplayerSessionCallbackProxy* UC_CreateMultiplayerSessionCallbackProxy::CreateMultiplayerSession(APlayerController* PlayerController)
{
	UC_CreateMultiplayerSessionCallbackProxy* Proxy = NewObject<UC_CreateMultiplayerSessionCallbackProxy>();
	Proxy->PlayerController = PlayerController;

	return Proxy;
}

void UC_CreateMultiplayerSessionCallbackProxy::Activate()
{
	const FUniqueNetIdPtr UserID = QueryIDFromPlayerController(PlayerController.Get());
	if (!UserID.IsValid())
	{
		OnFailure.Broadcast();
        return;
	}
	
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	CreateCompleteDelegateHandle = SessionSubsystem->OnMultiplayerSessionCreated.Add(CreateCompleteDelegate);
	const bool bSuccess = SessionSubsystem->CreateSession(UserID);
	if (!bSuccess)
	{
		SessionSubsystem->OnMultiplayerSessionCreated.Remove(CreateCompleteDelegateHandle);
		OnFailure.Broadcast();
	}
}

void UC_CreateMultiplayerSessionCallbackProxy::OnCreateMultiplayerSessionComplete(bool bSuccess) const
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	SessionSubsystem->OnMultiplayerSessionCreated.Remove(CreateCompleteDelegateHandle);
	if (bSuccess)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
