// Fill out your copyright notice in the Description page of Project Settings.


#include "Net/C_DestroyMultiplayerSessionCallbackProxy.h"

#include "SubSystem/C_MultiplayerSessionSubsystem.h"

UC_DestroyMultiplayerSessionCallbackProxy::UC_DestroyMultiplayerSessionCallbackProxy(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer),
DestroyCompleteDelegate(FOnDestroyMultiplayerSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroyMultiplayerSessionComplete))
{
	SessionSubsystem = UC_MultiplayerSessionSubsystem::Get();
}

UC_DestroyMultiplayerSessionCallbackProxy* UC_DestroyMultiplayerSessionCallbackProxy::DestroyMultiplayerSession(FName SessionName)
{
	UC_DestroyMultiplayerSessionCallbackProxy* Proxy = NewObject<UC_DestroyMultiplayerSessionCallbackProxy>();
	Proxy->ToDestroySessionName = SessionName;
	return Proxy;
}

void UC_DestroyMultiplayerSessionCallbackProxy::Activate()
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	DestroyCompleteDelegateHandle = SessionSubsystem->OnMultiplayerSessionDestroyed.Add(DestroyCompleteDelegate);
	const bool bSuccess = SessionSubsystem->DestroySession(ToDestroySessionName);
	if (!bSuccess)
	{
		SessionSubsystem->OnMultiplayerSessionDestroyed.Remove(DestroyCompleteDelegateHandle);
		OnFailure.Broadcast();
	}
}

void UC_DestroyMultiplayerSessionCallbackProxy::OnDestroyMultiplayerSessionComplete(bool bWasSuccessful)
{
	if (SessionSubsystem == nullptr)
	{
		OnFailure.Broadcast();
		return;
	}

	SessionSubsystem->OnMultiplayerSessionDestroyed.Remove(DestroyCompleteDelegateHandle);
	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
