// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/C_StartMultiplayerSessionCallbackProxy.h"

#include "SubSystem/C_MultiplayerSessionSubsystem.h"

UC_StartMultiplayerSessionCallbackProxy::UC_StartMultiplayerSessionCallbackProxy(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer),
StartCompleteDelegate(FOnStartMultiplayerSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartMultiplayerSessionComplete))
{
	SessionSubsystem = UC_MultiplayerSessionSubsystem::Get();
}

UC_StartMultiplayerSessionCallbackProxy* UC_StartMultiplayerSessionCallbackProxy::StartMultiplayerSession(FName SessionName)
{
	UC_StartMultiplayerSessionCallbackProxy* Proxy = NewObject<UC_StartMultiplayerSessionCallbackProxy>();
	Proxy->ToStartSessionName = SessionName;

	return Proxy;
}

void UC_StartMultiplayerSessionCallbackProxy::Activate()
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	StartCompleteDelegateHandle = SessionSubsystem->OnMultiplayerSessionStarted.Add(StartCompleteDelegate);
	const bool bSuccess = SessionSubsystem->StartSession(ToStartSessionName);
	if (!bSuccess)
	{
		SessionSubsystem->OnMultiplayerSessionStarted.Remove(StartCompleteDelegateHandle);
		OnFailure.Broadcast();
	}
}

void UC_StartMultiplayerSessionCallbackProxy::OnStartMultiplayerSessionComplete(bool bSuccess) const
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	SessionSubsystem->OnMultiplayerSessionStarted.Remove(StartCompleteDelegateHandle);
	if (bSuccess)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}


